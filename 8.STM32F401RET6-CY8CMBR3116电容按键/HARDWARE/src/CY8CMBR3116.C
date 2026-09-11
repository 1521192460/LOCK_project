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
    CY8CMBR3116_reg_config(1);
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
    }
    else if (key_temp == 0xff && key_flag == 0)
    {
        key_flag = 1;
    }
    return key_value;
}