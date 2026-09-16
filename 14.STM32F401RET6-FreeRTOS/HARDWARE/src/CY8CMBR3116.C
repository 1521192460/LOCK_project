#include "CY8CMBR3116.h"

//配置寄存器参数
const unsigned char CY8CMBR3116_RegPara[128] = {
                    
    0xFFu, 0x0Fu, 0xFBu, 0x0Fu, 0x00u, 0x00u, 0x00u, 0x00u,
    0xAAu, 0xAAu, 0xA9u, 0x00u, 0x80u, 0x80u, 0x80u, 0x80u,
    0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u, 0x80u,
    0x7Fu, 0x7Fu, 0x7Fu, 0x7Fu, 0x08u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x80u,
    0x05u, 0x00u, 0x00u, 0x02u, 0x00u, 0x02u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x1Eu, 0x00u,
    0x00u, 0x1Eu, 0x1Eu, 0x00u, 0x00u, 0x00u, 0x01u, 0x01u,
    0x00u, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu, 0xFFu,
    0xFFu, 0x00u, 0x00u, 0x00u, 0x20u, 0x03u, 0x01u, 0x63u,
    0x00u, 0x37u, 0x03u, 0x00u, 0x00u, 0x0Au, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0xA1u, 0xD5u
};





/**************************************************
 * 函数名:CY8CMBR3116_pin_init
 * 函数功能:CY8CMBR3116引脚初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *           CY9CMBR3116_RES-----PA1---通用推挽输出
 ***************************************************/
void CY8CMBR3116_pin_init(void)
{
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    //初始化引脚
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    //初始化iic1引脚
    iic1_pin_init();

    //初始化复位引脚
    CY8CMBR3116_RES_H;
}






/**************************************************
 * 函数名:CY8CMBR3116_write_data
 * 函数功能:CY8CMBR3116写入多字节数据
 * 函数参数：u8 reg_addr--要写入的寄存器地址
 *          u8 len--要写入的数据长度
 *          u8 *data--要写入的数据
 * 函数返回值：u8
 * 函数说明：
 *          发送数据不能太快也不能太慢，参考厂家示例代码，每个写入步骤都需要添加500us的延时
 ***************************************************/
u8 CY8CMBR3116_write_data(u8 reg_addr,u8 len,u8 *data)
{
START:
    //1.等待从器件响应
    iic1_start();//起始信号
    iic1_send_data(CY8CMBR3116_ADDR_WRITE);//发送写器件地址
    if(iic1_rec_ack() != 0)//响应失败就重新响应直到成功
    {
        goto START;
    }
    //2.写入寄存器的地址
    iic1_send_data(reg_addr);
    delay_us(500);//延时
    if(iic1_rec_ack() != 0)//接收应答失败就重新响应直到成功
    {
        return 1;
    }
    delay_us(500);//延时
    //3.写入数据
    while(len--)
    {
        iic1_send_data(*data++);
        delay_us(500);//延时
        if(iic1_rec_ack() != 0)//接收应答失败就重新响应直到成功
        {
            return 2;
        }
        delay_us(500);//延时
    }   
    iic1_stop();//发送停止信号
    return 0;
}



/**************************************************
 * 函数名:CY8CMBR3116_read_data
 * 函数功能:CY8CMBR3116读取多字节数据
 * 函数参数：u8 reg_addr--要读取的寄存器地址
 *          u8 len--要读取的数据长度
 *          u8 *data--要读取的数据
 * 函数返回值：u8
 * 函数说明：
 ***************************************************/
u8 CY8CMBR3116_read_data(u8 reg_addr,u8 len,u8 *data)
{
START1:
    //1.等待从器件响应
    iic1_start();//起始信号
    iic1_send_data(CY8CMBR3116_ADDR_WRITE);//发送写器件地址
    //判断是否响应
    if(iic1_rec_ack() != 0)//响应失败就重新响应直到成功
    {
        goto START1;
    }
    //2.发送寄存器地址
    iic1_send_data(reg_addr);
    if(iic1_rec_ack() != 0)//接收应答失败就重新响应直到成功
    {
        return 1;
    }
    iic1_stop();//发送停止信号
START2:
    //1.等待从器件响应
    iic1_start();//起始信号
    iic1_send_data(CY8CMBR3116_ADDR_READ);//发送读器件地址
    //判断是否响应
    if(iic1_rec_ack() != 0)//响应失败就重新响应直到成功
    {
        goto START2;
    }
    while(len--)
    {
        *data++ = iic1_rec_data();
        if(len == 0)
        {
            //最后一个字节就发送NACK
            iic1_send_ack(1);
        }
        else
        {
            //其他字节就发送ACK
            iic1_send_ack(0);
        }
    }
    iic1_stop();//发送停止信号
    return 0;
}


/**************************************************
 * 函数名:CY8CMBR3116_reg_config
 * 函数功能:CY8CMBR3116寄存器配置函数
 * 函数参数：
 *           u8 set_mode--0：根据芯片配置情况配置   1：自定义配置寄存器值
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void CY8CMBR3116_reg_config(u8 set_mode)
{
    u8 cnt = 0;    
    //1.获取芯片状态
    u8 sys_status = 0;
    CY8CMBR3116_read_data(0x8a,1,&sys_status);//读取系统配置标志，获取芯片状态
    (sys_status==0) ? (printf("芯片为出厂设置\r\n")) : (printf("芯片不为出厂设置\r\n"));
    //2.根据出厂状态判断是否要对进行配置
    if(sys_status == 0 && set_mode == 0)
    {
        return;//芯片已经被配置过，不需要再配置了
    }
    //3.从寄存器地址0x00开始发送128个字节数据
    u8 ret = 0;
REINIT:
    for(u8 i=0;i<10;i++)//防止配置失败，尝试10次
    {
        ret = CY8CMBR3116_write_data(0x00,128,(u8 *)CY8CMBR3116_RegPara);
        if(ret == 0)//成功配置就结束循环
            break;
    }
    if(ret == 0)
        printf("配置寄存器成功\r\n");
    else
    {
        printf("配置寄存器失败\r\n");
        return;
    }

    //4.配置指令寄存器，校验并且保存到非易失性存储器
    for(u8 i=0;i<10;i++)//防止配置失败，尝试10次
    {
        ret = CY8CMBR3116_write_data(0x86,1,(u8 *)2);
        if(ret == 0)//成功配置就结束循环
        break;
    }
    if(ret == 0)
        printf("配置指令寄存器成功\r\n");
    else
    {
        printf("配置指令寄存器失败\r\n");
        return;
    }
    delay_ms(300);//继续往下跑代码，需要等待300ms，确保配置完成
    //5.读取错误码寄存器，判断是否配置成功
    u8 err_code = 0;
    CY8CMBR3116_read_data(0x89,1,&err_code);
    switch(err_code)
    {
        case 0:printf("指令执行成功\r\n");break;
        case 253:printf("对闪存的写入操作失败\r\n");break;
        case 254:printf("CONFIG_CRC 所存储的配置 CRC 校验和与计算得到的配置 CRC 校验和不一致\r\n");break;
        case 255:printf("指令无效\r\n");break;
    }
    cnt++;
    if(err_code != 0)
    {
        if(cnt < 10)
        {
            goto REINIT;
        }
        else
        {
            printf("配置失败，10次尝试均失败\r\n");
            return;
        }
    }
    //6.配置成功，软件复位
    CY8CMBR3116_write_data(0x86,1,(u8 *)255);
    printf("总体配置成功\r\n");
}


/**************************************************
 * 函数名:CY8CMBR3116_init
 * 函数功能:CY8CMBR3116初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void CY8CMBR3116_init(void)
{
    //1.初始化引脚
    CY8CMBR3116_pin_init();
    //2.复位
    CY8CMBR3116_RES_L;
    delay_ms(100);
    CY8CMBR3116_RES_H;
    //3.芯片寄存器配置
    CY8CMBR3116_reg_config(0);
}


/**************************************************
 * 函数名:CY8CMBR3116_get_key
 * 函数功能:CY8CMBR3116获取按键值
 * 函数参数：u8 key--按键值
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
u8 CY8CMBR3116_get_key(void)
{
    //获取16位数据
    u8 button[2] = {0};
    //读取16位按键状态寄存器
    CY8CMBR3116_read_data(0xaa,2,button);
    u16 key = (button[0] << 8) | button[1];
    switch(key)
    {
        case 0x0800:return '1';
        case 0x1000:return '2';
        case 0x0001:return '3';
        case 0x0400:return '4';
        case 0x2000:return '5'; 
        case 0x0002:return '6';
        case 0x0100:return '7';
        case 0x8000:return '8';
        case 0x0008:return '9';
        case 0x0200:return '*';
        case 0x4000:return '0';
        case 0x0004:return '#';
    }
    return 0xff;
}



/**************************************************
 * 函数名:CY8CMBR3116_key_scan
 * 函数功能:CY8CMBR3116扫描按键
 * 函数参数：u8 key--按键值
 * 函数返回值：无
 * 函数说明：按一次按键只返回一次按键值
 ***************************************************/
u8 CY8CMBR3116_key_scan(void)
{
    static u8 key_flag = 1;  
    u8 key_temp = 0;
    key_temp = CY8CMBR3116_get_key();
    u8 key_value = 0xff;
    //按下按键，获取按键值然后锁定一次
    if(key_temp != 0xff && key_flag == 1)
    {
        key_flag = 0;
        key_value = key_temp;
        NV400F_send_data(0X2E);
    }
    else if (key_temp == 0xff && key_flag == 0)
    {
        key_flag = 1;
    }
    return key_value;
}



/**************************************************
 * 函数名:check_init_password
 * 函数功能:开机上电初始密码
 * 函数参数：u8 key--按键值
 * 函数返回值：无
 * 函数说明：
 *          在AT24C02中开辟一个空间作为初始密码标志位
 *          如果标志位为0，说明是第一次开机，需要设置初始密码
 *          如果标志位为1，说明不是第一次开机，不需要设置初始密码
 *          addr 1-21空间 用于存储初始密码
 *          addr 22空间 用于存储密码长度标志位
 ***************************************************/
void check_init_password(void)
{
    u8 pwd_flag = 0;  
    at24c02_read_byte(0,&pwd_flag);//读出初始密码标志位
    if(pwd_flag == 1)//不是第一次开机
    {
        return;
    }
    else//是第一次开机
    {
        lcd_show_zk_str("欢迎使用智能锁",0,0,32,0x0000,0xffff);
        NV400F_send_data(0x2d); 
        delay_ms(2000);
        lcd_clear(0,0,240,240,0xffff);
    }
    lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
    NV400F_send_data(0x0c);
    u8 key_value = 0;       //按键值
    u8 pwd_cnt = 0;            //密码长度计数器
    u8 pwd_input = 1;           //记录第几次输入密码
    u8 first_pwd[20] = {0};//第一次输入密码保存缓冲区
    u8 second_pwd[20] = {0};//第二次输入密码保存缓冲区
    u8 buff[50] = {0};//发送数据缓冲区
    //设置初始密码
    while(1)
    {
        key_value = CY8CMBR3116_key_scan();
        if(key_value != 0xff)
        {
            if(pwd_input == 1 )
            {
                first_pwd[pwd_cnt++] = key_value;
                if(key_value == '#')
                {
                    first_pwd[pwd_cnt-1] = '\0';
                    pwd_cnt = 0;
                    key_value = 0xff;
                    lcd_clear(0,0,240,240,0xffff);
                    lcd_show_zk_str("请再次确认密码",4,68,32,0x0000,0xffff);
                    NV400F_send_data(0x0d);
                    pwd_input = 2;          //切换到第二次输入密码
                }
            }
            else if(pwd_input == 2)
            {
                second_pwd[pwd_cnt++] = key_value;
                if(key_value == '#')
                {
                    second_pwd[pwd_cnt-1] = '\0';
                    sprintf((char *)buff,"AT+MQTTPUB=attributes,0,0,{\\\"doorpwd\\\":\\\"%s\\\"}\r\n",second_pwd);
                    if(strcmp((char *)first_pwd,(char *)second_pwd) == 0)
                    {
                        lcd_clear(0,0,240,240,0xffff);
                        lcd_show_zk_str("两次输入密码一致，设置成功",0,0,32,0x0000,0xffff);
                        WIFI_send_data(buff,2000);
                        NV400F_send_data(0x1c);
                        at24c02_write_byte(22,pwd_cnt-1);//写入密码长度标志位
                        at24c02_write_cross_page(1,pwd_cnt-1,second_pwd);//写入初始密码
                        at24c02_write_byte(0,1);//写入初始密码标志位
                        lcd_clear(0,0,240,200,0xffff);
                        return;
                    }
                    else
                    {                 
                        lcd_clear(0,0,240,240,0xffff);      
                        lcd_show_zk_str("两次输入密码不一致，重新输入",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0x1b);
                        delay_ms(500);
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
                        key_value = 0xff;
                        pwd_cnt = 0;
                        pwd_input = 1;
                    }
                }
            }
        }
    }
}

/**************************************************
 * 函数名:password_open_door
 * 函数功能:密码开门
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void password_open_door(void)
{
    static u8 input_pwd[20] = {0};    //输入密码缓冲区缓冲区
    u8 read_pwd[20] = {0};            //读取密码缓冲区缓冲区
    static u8 pwd_cnt = 0;            //密码长度计数器
    static u8 error_input = 0;        //记录第几次输入密码
    u8 key_value = 0xff;              //按键值
    u8 pwd_len = 0;                   //读取密码长度
    
    key_value = CY8CMBR3116_key_scan();
    if(key_value != 0xff)
    {   
        input_pwd[pwd_cnt++] = key_value;
        if(key_value == '#')
        {
            at24c02_read_byte(22,&pwd_len);//读取密码长度标志位
            input_pwd[pwd_cnt-1] = '\0';        //清除最后一位‘#’
            if(pwd_len == 0 || pwd_len > 19)
            {
                pwd_cnt = 0;
                memset(input_pwd,0,sizeof(input_pwd));
                key_value = 0xff;
                return;
            }
            at24c02_sequential_read(1,pwd_len,read_pwd);//读取密码
            if(strcmp((char *)input_pwd,(char *)read_pwd) == 0)
            {
                lcd_clear(0,0,240,240,0xffff);
                lcd_show_zk_str("开门成功，欢迎回家",0,0,32,0x0000,0xffff);
                delay_ms(100);
                NV400F_send_data(0x12);
                error_input = 0;
                LOCK_ON;
                delay_ms(2000);
                LOCK_OFF;
                memset(input_pwd,0,sizeof(input_pwd));     
            }
            else
            {
                lcd_clear(0,0,240,240,0xffff);
                lcd_show_zk_str("密码错误，开门失败",0,0,32,0x0000,0xffff);
                NV400F_send_data(0x13);
                delay_ms(1000);
                error_input++;
                if(error_input >= 3)
                {
                   NV400F_send_data(0x18);
                }

            }
            lcd_clear(0,0,240,240,0xffff);
            ui_flag = 0;//重置ui_flag
            pwd_cnt = 0;
        }  
    }
}


/**************************************************
 * 函数名:admin_password_check
 * 函数功能:管理员密码验证
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void admin_password_check(void)
{
    static u8 input_pwd[20] = {0};    //输入密码缓冲区缓冲区
    u8 read_pwd[20] = {0};            //读取密码缓冲区缓冲区
    static u8 pwd_cnt = 0;            //密码长度计数器
    u8 key_value = 0xff;              //按键值
    u8 pwd_len = 0;                   //读取密码长度
    
    key_value = CY8CMBR3116_key_scan();
    if(key_value != 0xff)
    {   
        input_pwd[pwd_cnt++] = key_value;
        if(key_value == '#')
        {
            at24c02_read_byte(34,&pwd_len);//读取管理员密码长度标志位
            input_pwd[pwd_cnt-1] = '\0';        //清除最后一位‘#’
            if(pwd_len == 0 || pwd_len > 19)
            {
                pwd_cnt = 0;
                memset(input_pwd,0,sizeof(input_pwd));
                key_value = 0xff;
                return;
            }
            at24c02_sequential_read(23,pwd_len,read_pwd);//读取管理员密码
            if(strcmp((char *)input_pwd,(char *)read_pwd) == 0)
            {
                lcd_clear(0,0,240,240,0xffff);
                lcd_show_zk_str("管理员密码正确，验证成功",0,0,32,0x0000,0xffff);
                delay_ms(1000);
                ui_flag = 0;//重置ui_flag
                lcd_clear(0,0,240,240,0xffff);
                page_flag = 3;//切换到管理员页面
                memset(input_pwd,0,sizeof(input_pwd));
                key_value = 0xff;
            }
            else
            {
                lcd_clear(0,0,240,240,0xffff);
                lcd_show_zk_str("管理员密码错误，验证失败",0,0,32,0x0000,0xffff);
                delay_ms(1000);
                lcd_clear(0,0,240,240,0xffff);
                page_flag = 2;//切换到管理员验证页面（重新输入密码）
                ui_flag = 0;//重置ui_flag
            }
            pwd_cnt = 0;
            memset(input_pwd,0,sizeof(input_pwd));
            key_value = 0xff;
        }  
        if(key_value == '*')
        {
            page_flag = 1;//切换到开锁页面
            ui_flag = 0;//重置ui_flag
            memset(input_pwd,0,sizeof(input_pwd));
            lcd_clear(0,0,240,240,0xffff);
            key_value = 0xff;
        }
    }
}


/**************************************************
 * 函数名:change_password_door
 * 函数功能:修改开门密码
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void change_password_door(void)
{
    u8 key_value = 0xff;                //按键值
    u8 first_input[20] = {0};           //第一个输入密码缓冲区缓冲区
    u8 second_input[20] = {0};          //第二个输入密码缓冲区缓冲区
    u8 pwd_len = 0;                     //密码长度
    u8 pwd_cnt = 1;                     //记录第几次输入密码
    u8 buff[60] = {0};
    lcd_clear(0,0,240,240,0xffff);
    lcd_show_zk_str("按*键返回",0,206,32,0x0000,0xffff);
    lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
    NV400F_send_data(0X0C);     //请输入新密码
    while(1)
    {
        key_value = CY8CMBR3116_key_scan();
        if(key_value != 0xff)
        {
            if(pwd_cnt == 1)
            {
                first_input[pwd_len++] = key_value;
                if(key_value == '#')
                {
                    first_input[pwd_len-1] = '\0';
                    lcd_clear(0,0,240,200,0xffff);
                    pwd_len = 0;
                    pwd_cnt++;
                    lcd_show_zk_str("再次输入新密码",4,68,32,0x0000,0xffff);
                    NV400F_send_data(0X0d);     //请再次输入新密码
                    pwd_cnt = 2;            //切换第二次输入
                }
            }
            else if(pwd_cnt == 2)
            {
                second_input[pwd_len++] = key_value;
                if(key_value == '#')
                {
                    second_input[pwd_len-1] = '\0';
                    sprintf((char *)buff,"AT+MQTTPUB=attributes,0,0,{\\\"doorpwd\\\":\\\"%s\\\"}\r\n",second_input);
                    //判断第一次输入和第二次输入是否一致
                    if(strcmp((char *)first_input,(char *)second_input) == 0)
                    {
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("密码修改成功",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0X1C);//操作成功语言
                        at24c02_write_cross_page(1,pwd_len-1,second_input);//写入开门密码
                        at24c02_write_byte(22,pwd_len-1);//写开门密码长度标志位
                        lcd_clear(0,0,240,240,0xffff);
                        WIFI_send_data((u8 *)buff,3000);
                        ui_flag = 0;//重置ui_flag
                        page_flag = 3;//切换到管理员界面
                        return;
                    }
                    else
                    {
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("两次输入密码不一致，修改失败",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0X1b);//密码不一致语音
                        delay_ms(500);
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
                        NV400F_send_data(0X0C);     //请输入新密码
                        key_value = 0xff;
                        pwd_cnt = 1;
                        pwd_len = 0;
                    }
                }
            }
            if(key_value == '*')
            {
                page_flag = 3;//切换到管理员界面
                ui_flag = 0;//重置ui_flag
                memset(first_input,0,sizeof(first_input));
                memset(second_input,0,sizeof(second_input));
                key_value = 0xff;
                lcd_clear(0,0,240,240,0xffff);
                return;
            }
        }
    }
}





/**************************************************
 * 函数名:change_password_admin
 * 函数功能:修改管理员密码
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void change_password_admin(void)
{
    u8 key_value = 0xff;                //按键值
    u8 first_input[20] = {0};           //第一个输入密码缓冲区缓冲区
    u8 second_input[20] = {0};          //第二个输入密码缓冲区缓冲区
    u8 pwd_len = 0;                     //密码长度
    u8 pwd_cnt = 1;                     //记录第几次输入密码
    u8 buff[60] = {0};
    lcd_clear(0,0,240,240,0xffff);
    lcd_show_zk_str("按*键返回",0,206,32,0x0000,0xffff);
    lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
    NV400F_send_data(0X0C);     //请输入新密码
    while(1)
    {
        key_value = CY8CMBR3116_key_scan();
        if(key_value != 0xff)
        {
            if(pwd_cnt == 1)
            {
                first_input[pwd_len++] = key_value;
                if(key_value == '#')
                {
                    first_input[pwd_len-1] = '\0';
                    lcd_clear(0,0,240,200,0xffff);
                    pwd_len = 0;
                    pwd_cnt++;
                    lcd_show_zk_str("再次输入新密码",4,68,32,0x0000,0xffff);
                    NV400F_send_data(0X0d);     //请再次输入新密码
                    pwd_cnt = 2;            //切换第二次输入
                }
            }
            else if(pwd_cnt == 2)
            {
                second_input[pwd_len++] = key_value;
                if(key_value == '#')
                {
                    second_input[pwd_len-1] = '\0';
                    sprintf((char *)buff,"AT+MQTTPUB=attributes,0,0,{\\\"adminpwd\\\":\\\"%s\\\"}\r\n",second_input);
                    //判断第一次输入和第二次输入是否一致
                    if(strcmp((char *)first_input,(char *)second_input) == 0)
                    {
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("密码修改成功",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0X1C);//操作成功语言
                        at24c02_write_cross_page(23,pwd_len-1,second_input);//写入管理员密码
                        at24c02_write_byte(34,pwd_len-1);//写管理员密码长度标志位
                        WIFI_send_data((u8 *)buff,3000);
                        lcd_clear(0,0,240,240,0xffff);
                        ui_flag = 0;//重置ui_flag
                        page_flag = 3;//切换到管理员界面
                        return;
                    }
                    else
                    {
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("两次输入密码不一致，修改失败",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0X1b);//密码不一致语音
                        delay_ms(500);
                        lcd_clear(0,0,240,200,0xffff);
                        lcd_show_zk_str("请输入新密码",24,68,32,0x0000,0xffff);
                        NV400F_send_data(0X0C);     //请输入新密码
                        key_value = 0xff;
                        pwd_cnt = 1;
                        pwd_len = 0;
                    }
                }
            }
            if(key_value == '*')
            {
                page_flag = 3;//切换到管理员界面
                ui_flag = 0;//重置ui_flag
                memset(first_input,0,sizeof(first_input));
                memset(second_input,0,sizeof(second_input));
                key_value = 0xff;
                lcd_clear(0,0,240,240,0xffff);
                return;
            }
        }
    }
}