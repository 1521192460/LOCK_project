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
    (sys_status==0) ? (lcd_show_zk_str("芯片为出厂设置",0,0,16,0x0000,0xffff)) : (lcd_show_zk_str("芯片不为出厂设置",0,0,16,0x0000,0xffff));
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
        lcd_show_zk_str("配置寄存器成功",0,16,16,0x0000,0xffff);
    else
    {
        lcd_show_zk_str("配置寄存器失败",0,16,16,0x0000,0xffff);
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
        lcd_show_zk_str("配置指令寄存器成功",0,32,16,0x0000,0xffff);
    else
    {
        lcd_show_zk_str("配置指令寄存器失败",0,32,16,0x0000,0xffff);
        return;
    }
    delay_ms(300);//继续往下跑代码，需要等待300ms，确保配置完成
    //5.读取错误码寄存器，判断是否配置成功
    u8 err_code = 0;
    CY8CMBR3116_read_data(0x89,1,&err_code);
    switch(err_code)
    {
        case 0:lcd_show_zk_str("指令执行成功",0,48,16,0x0000,0xffff);break;
        case 253:lcd_show_zk_str("对闪存的写入操作失败",0,48,16,0x0000,0xffff);break;
        case 254:lcd_show_zk_str("CONFIG_CRC 所存储的配置 CRC 校验和与计算得到的配置 CRC 校验和不一致",0,48,16,0x0000,0xffff);break;
        case 255:lcd_show_zk_str("指令无效",0,48,16,0x0000,0xffff);break;
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
    lcd_show_zk_str("总体配置成功",0,64,16,0x0000,0xffff);
    lcd_clear(0,0,240,240,0xffff);
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
 * 函数名:CY8CMBR3116_get_button
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
 *          addr 1-6空间 用于存储初始密码
 *          addr 7空间 用于存储密码长度标志位
 ***************************************************/
void check_init_password(void)
{
    u8 pwd_flag = 0;  
    at24c02_read_byte(0,&pwd_flag);//读出初始开机标志位
    if(pwd_flag == 1)//不是第一次开机
    {
        return;
    }
    else//是第一次开机
    {
        lcd_show_zk_str("欢迎使用智能锁          ",0,0,32,0x0000,0xffff);
        NV400F_send_data(0x2d); 
        delay_ms(2000);
    }
    lcd_show_zk_str("设置开门密码           ",0,0,32,0x0000,0xffff);
    NV400F_send_data(0x01);
    u8 key_value = 0;       //按键值
    u8 pwd_cnt = 0;            //密码长度计数器
    u8 pwd_input = 1;           //记录第几次输入密码
    u8 first_pwd[6] = {0};//第一次输入密码保存缓冲区
    u8 second_pwd[6] = {0};//第二次输入密码保存缓冲区
    //设置初始密码
    while(1)
    {
        key_value = CY8CMBR3116_key_scan();
        if(key_value != 0xff)
        {
            if(pwd_input == 1 )//第一次设置密码
            {
                first_pwd[pwd_cnt++] = key_value;
                if(pwd_cnt == 6)
                {
                    pwd_cnt = 0;
                    key_value = 0xff;
                    lcd_show_zk_str("请再次输入确认密码           ",0,0,32,0x0000,0xffff);
                    NV400F_send_data(0x31);
                    delay_ms(200);
                    pwd_input = 2;          //切换到第二次输入密码
                }
            }
            else if(pwd_input == 2)
            {
                second_pwd[pwd_cnt++] = key_value;
                if(pwd_cnt == 6)
                {
                    if(strcmp((char *)first_pwd,(char *)second_pwd) == 0)
                    {
                        lcd_show_zk_str("两次输入密码一致，设置成功  ",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0x1c);
                        delay_ms(500);
                        at24c02_write_byte(7,pwd_cnt);//写入密码长度标志位
                        at24c02_write_cross_page(1,pwd_cnt,second_pwd);//写入初始密码
                        return;
                    }
                    else
                    {                       
                        lcd_show_zk_str("两次输入密码不一致，重新输入",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0x1b);
                        delay_ms(500);
                        lcd_show_zk_str("请设置初始密码              ",0,0,32,0x0000,0xffff);
                        key_value = 0xff;
                        pwd_cnt = 0;
                        pwd_input = 1;
                    }
                }
            }
        }
    }
}
/****************************************************
 * 函数名:set_admin_password
 * 函数功能:设置管理员密码
 * 函数参数：void
 * 函数返回值：无
 * 函数说明：
 *          addr 8-13空间 用于存储管理员密码
 *          addr 14空间 用于存储密码长度标志位
 *          如果管理员密码和初始密码一致就要重新设置管理员密码
 ***************************************************/
void set_admin_password(void)
{
    u8 pwd_flag = 0;  
    at24c02_read_byte(0,&pwd_flag);
    if(pwd_flag == 1)
    {
        return;
    }
    else
    {
        lcd_show_zk_str("设置管理员密码              ",0,0,32,0x0000,0xffff);
        NV400F_send_data(0x30); 
        delay_ms(2000);
    }
    u8 key_value = 0;
    u8 pwd_cnt = 0;
    u8 pwd_input = 1;
    u8 first_pwd[6] = {0};
    u8 second_pwd[6] = {0};
    u8 init_pwd[6] = {0};
    while(1)
    {
        key_value = CY8CMBR3116_key_scan();
        if(key_value != 0xff)
        {
            if(pwd_input == 1)
            {
                first_pwd[pwd_cnt++] = key_value;
                if(pwd_cnt == 6)
                {
                    pwd_cnt = 0;
                    key_value = 0xff;
                    lcd_show_zk_str("请再次输入确认密码           ",0,0,32,0x0000,0xffff);
                    NV400F_send_data(0x31);
                    delay_ms(300);
                    pwd_input = 2;
                    while(CY8CMBR3116_get_key() != 0xff);
                    CY8CMBR3116_key_scan();
                }
            }
            else if(pwd_input == 2)
            {
                second_pwd[pwd_cnt++] = key_value;
                if(pwd_cnt == 6)
                {
                    at24c02_sequential_read(1,6,init_pwd);
                    if(strcmp((char *)first_pwd,(char *)second_pwd) != 0)
                    {
                        lcd_show_zk_str("两次输入密码不一致，重新输入",0,0,32,0x0000,0xffff);
                        NV400F_send_data(0x1b);
                        delay_ms(500);
                        lcd_show_zk_str("请设置管理员密码            ",0,0,32,0x0000,0xffff);
                        key_value = 0xff;
                        pwd_cnt = 0;
                        pwd_input = 1;
                    }
                    else 
                    {
                        if(strcmp((char *)second_pwd,(char *)init_pwd) == 0)
                        {
                            lcd_show_zk_str("与初始密码一致，重新输入      ",0,0,32,0x0000,0xffff);
                            NV400F_send_data(0x17);
                            delay_ms(500);
                            lcd_show_zk_str("请设置管理员密码           ",0,0,32,0x0000,0xffff);
                            key_value = 0xff;
                            pwd_cnt = 0;
                            pwd_input = 1;
                        }
                        else if(strcmp((char *)first_pwd,(char *)second_pwd) == 0)
                        {
                            lcd_show_zk_str("两次输入密码一致，设置成功  ",0,0,32,0x0000,0xffff);
                            delay_ms(500);
                            NV400F_send_data(0x1c);
                            at24c02_write_byte(14,pwd_cnt);
                            at24c02_write_cross_page(8,pwd_cnt,second_pwd);
                            at24c02_write_byte(0,1);
                            lcd_show_zk_str("请输入密码：                ",0,0,32,0x0000,0xffff);
                            return;
                        }
                    }
                }
            }
        }
    }
}





/**************************************************
 * 函数名:password_open_door
 * 函数功能:密码开门
 * 函数参数：u8 key--按键值
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
u8 password_open_door(void)
{
    static u8 input_pwd[6] = {0};
    static u8 pwd_cnt = 0;

    u8 key_value;
    u8 user_pwd[6] = {0};
    u8 admin_pwd[6] = {0};
    static u8 err_cnt = 0;

    key_value = CY8CMBR3116_key_scan();

    if(key_value == 0xff)
    {
        return AUTH_NONE;//无按键
    }

    input_pwd[pwd_cnt++] = key_value;

    /* 收集满 6 位密码后自动验证 */
    if(pwd_cnt < 6)
    {
        return AUTH_NONE;//密码未输入满 6 位
    }

    /* 读取用户密码：AT24C02 地址 1~6 */
    at24c02_sequential_read(1, 6, user_pwd);

    /* 读取管理员密码：AT24C02 地址 8~13 */
    at24c02_sequential_read(8, 6, admin_pwd);

    /*
     * 管理员密码优先判断。
     * 如果两种密码相同，系统会进入管理员页面。
     */
    if(memcmp(input_pwd, admin_pwd, 6) == 0)
    {
        pwd_cnt = 0;
        memset(input_pwd, 0, sizeof(input_pwd));

        lcd_show_zk_str("管理员认证成功          ",0, 0, 32, 0x0000, 0xffff);
        err_cnt = 0;
        delay_ms(500);

        return AUTH_ADMIN_OK;
    }

    /* 用户密码验证 */
    if(memcmp(input_pwd, user_pwd, 6) == 0)
    {
        pwd_cnt = 0;
        memset(input_pwd, 0, sizeof(input_pwd));

        lcd_show_zk_str("密码正确，开门成功      ",0, 0, 32, 0x0000, 0xffff);

        NV400F_send_data(0x12);
        err_cnt = 0;
        LOCK_ON;
        delay_ms(1000);
        LOCK_OFF;

        lcd_show_zk_str("请输入密码：            ",0, 0, 32, 0x0000, 0xffff);

        return AUTH_USER_OK;
    }

    /* 密码错误 */
    pwd_cnt = 0;
    memset(input_pwd, 0, sizeof(input_pwd));

    lcd_show_zk_str("密码错误                ",0, 0, 32, 0x0000, 0xffff);
    err_cnt++;
    NV400F_send_data(0x13);
    delay_ms(1000);

    lcd_show_zk_str("请输入密码：            ",0, 0, 32, 0x0000, 0xffff);
    //大于三次错误，报警
    if(err_cnt >= 3)
    {
        NV400F_send_data(0x18);
    }
    return AUTH_FAILED;
}



