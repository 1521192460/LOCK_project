#include "MG200.h"


/*****************************
 * 函数名：MG200_init
 * 函数功能：指纹200引脚初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *          PC1--通用推挽输出
 *          PC0--通用输入
********************************/
void MG200_init(void)
{
    //串口6初始化
    usart6_init(115200);
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC,&GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOC,&GPIO_InitStruct);
    //设置引脚电平
    MG200_POWER_ON;
    //等待模块上电完成
    delay_ms(500);
}



/*****************************
 * 函数名：MG200_send_packet
 * 函数功能：发送数据包
 * 函数参数：
 *          u8 cmd--指令
 *          u8 param--指令参数
 * 函数返回值：无
 * 函数说明：
********************************/   
void MG200_send_packet(u8 cmd,u8 param)
{
    usart6_send_byte(0x6c);//发送起始码
    usart6_send_byte(0x63);//发送地址
    usart6_send_byte(0x62);//接收地址
    usart6_send_byte(cmd);//发送指令
    usart6_send_byte(param);//发送指令参数
    usart6_send_byte(0x00);//发送预留字节
    u8 check_sum = (0x63 + 0x62 + cmd + param + 0x00) & 0xff;
    usart6_send_byte(check_sum);//发送校验和

    u6.flag = 0;//模块上电会发送其他数据，提前清零，方便接收MG200数据包
}


/*****************************
 * 函数名：MG200_rec_packet
 * 函数功能：接收数据包
 * 函数参数：
 *          u8 cmd--指令
 *          u8 *param--指令参数
 *          u8 *result--结果
 * 函数返回值：u8
 * 函数说明：
********************************/   
u8 MG200_rec_packet(u8 cmd,u8 *param,u8* result)
{
    //等待串口接收完数据
    while(!u6.flag);
    if(u6.buff[0]!=0x6c)//起始位判断
        return 1;
    if(u6.buff[1]!=0x62)//发送地址判断
        return 2;
    if(u6.buff[2]!=0x63)//接收地址判断
        return 3;
    if(u6.buff[3]!=cmd)//指令判断
        return 4;
    //接收结果
    *result = u6.buff[4];
    //接收参数
    *param = u6.buff[5];
    if(u6.buff[6]!=0x00)
        return 5;
    u8 check_sum = (0x62 + 0x63 + cmd + *result + *param + 0x00) & 0xff;
    if(u6.buff[7]!=check_sum)
        return 6;
    return 0;  

}

/*****************************
 * 函数名：MG200_get_fingerprint
 * 函数功能：抓取指纹图像和提取特征点
 * 函数参数：u8 capture_cnt--自定义注册指纹次数
 * 函数返回值：
 *              u8--指纹模块返回的参数，表示抓取指纹图像和提取特征点的结果
 * 函数说明：
********************************/  
u8 MG200_get_fingerprint(u8 capture_cnt)
{
    u8 result = 0;
    u8 param = 0;
    printf("请将手指放在指纹传感器上\r\n");
    while(!MG200_DETECT);
    //发送抓取指纹图像指令
    MG200_send_packet(0x51,capture_cnt);
    //判断接收数据包
    if(MG200_rec_packet(0x51,&param,&result) == 0)
    {
        switch(result)
        {
            case 0x00: printf("抓取指纹图像及特征量提取成功\r\n");break;
            case 0xB1: printf("指纹过小(small finger)\r\n");break;
            case 0xB2: printf("无指纹 (no finger)\r\n");break;
            case 0xB3: printf("指纹位置过于靠左，需要将手指靠右边摁按（left finger）\r\n");break;
            case 0xB4: printf("指纹位置过于靠右，需要将手指靠左边摁按（right finger）\r\n");break;
            case 0xB5: printf("指纹位置过于靠上，需要将手指靠下边摁按（up finger）\r\n");break;
            case 0xB6: printf("指纹位置过于靠下，需要将手指靠上边摁按 (down finger)\r\n");break;
            case 0xB7: printf("湿手指 （指纹表面出汗或者水渍过多） (wet finger)\r\n");break;
            case 0xB8: printf("干燥手指(dry finger)\r\n");break;    
            case 0xC0: printf("特征提取错误\r\n");break;
            case 0xC2: printf("特征提取错误 (图像质量差)\r\n");break;
            default: printf("抓取指纹图像及特征量提取错误\r\n");break;
        }
        return result;
    }
    else
    {
        printf("提取指纹图像失败\r\n");
        return 1;
    }
}


/*****************************
 * 函数名：MG200_enroll
 * 函数功能：注册用户指纹
 * 函数参数：u8 id--用户id号
 * 函数返回值：
 *              u8--注册结果
 * 函数说明：
********************************/  
u8 MG200_enroll(u8 id)
{
    u8 ret = 0;
    u8 param = 0;
    u8 result = 0;
    //1.手册要求需要提取特征图像3-5次
    do
        ret = MG200_get_fingerprint(0x00);//第1次提取
    while(ret != 0);
    do
        ret = MG200_get_fingerprint(0x01);//第2次提取
    while(ret != 0);
    do
        ret = MG200_get_fingerprint(0x02);//第3次提取
    while(ret != 0);
    //2.发送注册指纹数据包----参数：0x00表示注册的id号，00为随机分配
    MG200_send_packet(0x7f,id);
    //3.接收注册结果数据包
    if(MG200_rec_packet(0x7f,&param,&result) == 0)
    {
        switch(result)
        {
            case 0x00: printf("注册成功\r\n");break;
            case 0x83: printf("ID 错误(ID < 0 或者 ID > 最大用户数)或者通信错误\r\n");break;
            case 0x91: printf("注册失败(用户区域已满)\r\n");break;
            case 0x93: printf("已经注册的 ID\r\n");break;
            case 0x94: printf("指纹提取次数 < 3\r\n");break;
        }
        return result;
    }
    else
    {
        printf("注册失败\r\n");
        return 1;
    }
}


/*****************************
 * 函数名：MG200_match
 * 函数功能：匹配用户指纹
 * 函数参数：无
 * 函数返回值：
 *              u8--匹配结果，返回id号
 * 函数说明：
********************************/  
u8 MG200_match(void)
{
    u8 ret = 0;
    u8 param = 0;
    u8 result = 0;
    //1.先采集一次指纹图像直到成功为止
    do
        ret = MG200_get_fingerprint(0x00);//第1次提取
    while(ret != 0);
    //2.发送匹配指令
    MG200_send_packet(0x71,0x00);
    //3.接收匹配结果数据包
    if(MG200_rec_packet(0x71,&param,&result) == 0)//如果接收成功
    {
        switch(result)
        {
            case 0x00: printf("匹配成功，用户ID：%d\r\n",param);break;
            case 0x92: printf("匹配失败\r\n");break;
        }
        return param;
    }
    else
    {
        printf("通信错误\r\n");
        return 1;
    }
}



/*****************************
 * 函数名：MG200_erase
 * 函数功能：擦除用户指纹
 * 函数参数：u8 id--用户id号：0x01-0x64
 * 函数返回值：
 *              u8--擦除结果
 * 函数说明：
********************************/  
u8 MG200_erase(u8 id)
{
    u8 param = 0;
    u8 result = 0;
    //1.发送删除指令
    MG200_send_packet(0x73,id);
    //2.接收数据包
    if(MG200_rec_packet(0x73,&param,&result) == 0)
    {
        switch(result)
        {
            case 0x00: printf("删除成功\r\n");break;
            case 0x83: printf("参数错误(ID≤0 或者 ID > 最大用户数)\r\n");break;
            case 0x90: printf("未注册的用户\r\n");break;
            case 0xFF: printf("写入 ROM 错误\r\n");break;
        }
        return result;
    }
    return 0;
}


/*****************************
 * 函数名：MG200_erase_all
 * 函数功能：擦除所有用户指纹
 * 函数参数：无
 * 函数返回值：
 *              u8--擦除结果
 * 函数说明：
********************************/  
u8 MG200_erase_all(void)
{
    u8 param = 0;
    u8 result = 0;
    //1.发送擦除所有用户指纹指令
    MG200_send_packet(0x54,0x00);
    //2.接收数据包
    if(MG200_rec_packet(0x54,&param,&result) == 0)
    {
        switch(result)
        {
            case 0x00 : printf("删除全部用户指纹成功\r\n");break;
            case 0x90 : printf("删除失败 (注册的用户数为 0 )\r\n");break;
            default: printf("删除失败\r\n");
        }
        return result;
    }
    return 0;
}



/*****************************
 * 函数名：MG200_get_user_num
 * 函数功能：获取注册指纹数量
 * 函数参数：无
 * 函数返回值：
 *              u8--注册指纹数量，
 *              成功: 0-100 失败: 00h
 * 函数说明：
********************************/  
u8 MG200_get_user_num(void)
{
    u8 param = 0;
    u8 result = 0;
    //1.发送获取注册指纹数量指令
    MG200_send_packet(0x55,0x00);
    //2.接收数据包
    if(MG200_rec_packet(0x55,&param,&result) == 0)
    {
        switch(result)
        {
            case 0x00: printf("注册用户数：%d\r\n",param);break;
            default: printf("获取失败\r\n");break;
        }
        return param;
    }
    return 0;
}



/*****************************
 * 函数名：MG200_register
 * 函数功能：注册指纹页面+功能并且将指纹id写入at24c02指定地址
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ************************************/
void MG200_register(void)
{   
    static u8 id_enroll = 1;
    u8 confirm = 0;
    lcd_clear(0,0,240,240,0xffff);
    lcd_show_zk_str("采集指纹",56,0,32,0x0000,0xffff);
    lcd_show_zk_str("请放手指",56,98,32,0x0000,0xffff);
    NV400F_send_data(0X10);//请放手指语音
    do
    {
        confirm = MG200_enroll(id_enroll);
        if(confirm == 0)
        {
            lcd_show_zk_str("注册成功",56,98,32,0x0000,0xffff);
            NV400F_send_data(0X1c);//操作成功
            delay_ms(1000);
        }
    }while(confirm != 0);
    // printf("模块注册用户ID：%d\r\n",id_enroll);
    // at24c02_write_cross_page(34+id_enroll,1,&id_enroll);
    // u8 id;
    // at24c02_sequential_read(34+id_enroll,1,&id);
    // printf("芯片注册用户ID：%d\r\n",id);
    id_enroll++;
    if(id_enroll >= 10)
    {
        id_enroll = 1;
    }
    lcd_clear(0,0,240,240,0xffff);
    page_flag = 3;
    ui_flag = 0;
}
