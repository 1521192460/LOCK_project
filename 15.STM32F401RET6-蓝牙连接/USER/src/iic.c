#include "iic.h"

/**************************AT24C02 I2C初始化************************************************/


/**************************************************
 * 函数名:iic3_pin_init
 * 函数功能:iic3引脚初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *          AT24CXX_IIC3_SCK---PA8---通用推挽输出
 *          AT24CXX_IIC3_SDA---PC9---开漏输出
 ***************************************************/
void iic3_pin_init(void)
{
    /*时钟使能*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOC, ENABLE);

    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    //初始电平状态
    IIC3_SDA_H;
    IIC3_SCK_H;

}


/******************************
 * 函数名:iic3_start
 * 函数功能:iic3发送起始信号
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ******************************/
void iic3_start(void)
{
    IIC3_SCK_L;
        //延时
        delay_us(IIC3_DELAY);
    //注意：在起始信号和停止信号中，先动数据线
    //拉高数据线
    IIC3_SDA_H;
    //拉高时钟线
    IIC3_SCK_H;
    //延时
    delay_us(IIC3_DELAY);
    //拉低数据线，产生起始信号
    IIC3_SDA_L;
    //延时
    delay_us(IIC3_DELAY);
}

/******************************
 * 函数名:iic3_stop
 * 函数功能:iic3发送停止信号
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ******************************/
void iic3_stop(void)
{
    IIC3_SCK_L;
        //延时
        delay_us(IIC3_DELAY);
    //拉低数据线
    IIC3_SDA_L;
    //拉高时钟线
    IIC3_SCK_H;
    //延时
    delay_us(IIC3_DELAY);
    //拉高数据线
    IIC3_SDA_H;
    //延时
    delay_us(IIC3_DELAY);
}

/******************************
 * 函数名:iic3_send_data
 * 函数功能:iic3发送数据
 * 函数参数：u8 data
 * 函数返回值：无
 * 函数说明：
 ******************************/
void iic3_send_data(u8 data)
{
    //循环发送一字节数据
    for(u8 i=0;i<8;i++)
    {
        //拉低时钟线，主机准备发送数据
        IIC3_SCK_L;
        //延时
        delay_us(IIC3_DELAY);
        //判断高位数据是否为1
        if(data & (0x80 >> i))
            IIC3_SDA_H;         //拉高数据线，发送1
        else
            IIC3_SDA_L;         //拉低数据线，发送0
        //延时
        delay_us(IIC3_DELAY);
        //拉高时钟线，从机接收数据
        IIC3_SCK_H;
        //延时
        delay_us(IIC3_DELAY);
    }
}


/******************************
 * 函数名:iic3_rec_data
 * 函数功能:iic3接收数据
 * 函数参数：无
 * 函数返回值：u8--接收数据
 * 函数说明：
 ******************************/
u8 iic3_rec_data(void)
{
    u8 data = 0;
    //拉高数据线切换输入模式
    IIC3_SDA_H;
    for(u8 i=0;i<8;i++)
    {
        //拉低时钟线，从机向主机发送数据
        IIC3_SCK_L;
        //延时
        delay_us(IIC3_DELAY);//建立+保持时间
        //拉高时钟线，主机准备接收数据
        IIC3_SCK_H;
        //延时
        delay_us(IIC3_DELAY);//高电平保持时间
        //读取数据线电平
        data <<= 1;
       if(IIC3_SDA_IN)
          data |= 1;
       //高电平保持时间
       delay_us(IIC3_DELAY);
    }
    return data;
}

/******************************
 * 函数名:iic3_send_ack
 * 函数功能:iic3发送应答信号
 * 函数参数：u8 ack
 * 函数返回值：无
 * 函数说明：
 *          1为非应答信号，0为应答信号
 ******************************/
void iic3_send_ack(u8 ack)
{
    //拉低时钟线
    IIC3_SCK_L;
    //延时，时钟线低电平建立时间
    delay_us(IIC3_DELAY);
    //发送应答信号
    if(ack)
        IIC3_SDA_H;             //拉高数据线，产生非应答信号
    else
        IIC3_SDA_L;             //拉低数据线，产生应答信号
    //延时，时钟线低电平保持时间
    delay_us(IIC3_DELAY);
    //拉高时钟线
    IIC3_SCK_H;
    //延时，保持时钟线高电平
    delay_us(IIC3_DELAY);

    //拉低时钟线，保证时钟周期完整
    IIC3_SCK_L;
    //延时
    delay_us(IIC3_DELAY);
}

/******************************
 * 函数名:iic3_rec_ack
 * 函数功能:iic3接收应答信号
 * 函数参数：无
 * 函数返回值：u8--接收应答信号
 * 函数说明：
 *          1为非应答信号，0为应答信号
 ******************************/
u8 iic3_rec_ack(void)
{
    u8 ack = 0;
    IIC3_SCK_L;                  //拉低时钟线，从机发送应答信号
    //延时
    delay_us(IIC3_DELAY);
    IIC3_SCK_H;                  //拉高时钟线，主机接收应答信号
    //延时
    delay_us(IIC3_DELAY);
    IIC3_SDA_H;                  //拉高数据线，切换输入模式
    //判断应答信号
    if(IIC3_SDA_IN)
    {
        ack = 1;
    }
    //延时保持时钟线高电平
    delay_us(IIC3_DELAY);

    //拉低时钟线，保证时钟周期完整
    IIC3_SCK_L;
    //延时
    delay_us(IIC3_DELAY);
    //返回应答信号
    return ack;
}


/**************************************CY8CMBR3116 I2C初始化************************************************/

/**************************************************
 * 函数名:iic1_pin_init
 * 函数功能:iic1引脚初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *           CY9CMBR3116_SCK-----PB6---通用推挽输出
 *           CY9CMBR3116_SDA-----PB7---开漏输出
 ***************************************************/
void iic1_pin_init(void)
{
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    //初始化引脚
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    //初始电平状态
    IIC1_SCK_H;
    IIC1_SDA_H;
}

/**************************************************
 * 函数名:iic1_start
 * 函数功能:iic1起始信号
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void iic1_start(void)
{
    IIC1_SDA_H;//拉高数据线
    IIC1_SCK_H;//拉高时钟线
    delay_us(IIC1_DELAY);//延时
    IIC1_SDA_L;//拉低时钟线
    delay_us(IIC1_DELAY);//延时,保持数据线低电平
}

/**************************************************
 * 函数名:iic1_stop
 * 函数功能:iic1停止信号
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void iic1_stop(void)
{
    IIC1_SDA_L;//拉低数据线
    IIC1_SCK_H;//拉高时钟线
    delay_us(IIC1_DELAY);//延时
    IIC1_SDA_H;//拉高数据线
    delay_us(IIC1_DELAY);//延时,保持数据线高电平
}

/**************************************************
 * 函数名:iic1_rec_ack
 * 函数功能:iic1接收应答信号
 * 函数参数：无
 * 函数返回值：u8--接收应答信号
 * 函数说明：
 ***************************************************/
u8 iic1_rec_ack(void)
{
    u8 ack = 0;
    //拉低时钟线
    IIC1_SCK_L;
    delay_us(IIC1_DELAY);//延时
    //拉高时钟线
    IIC1_SCK_H;
    delay_us(IIC1_DELAY);//延时
    IIC1_SDA_H;//切换输出模式
    if(IIC1_SDA_IN)
        ack = 1;
    delay_us(IIC1_DELAY);//延时

    IIC1_SCK_L;//拉低时钟线，保证周期完整
    delay_us(IIC1_DELAY);//延时
    return ack;
}


/**************************************************
 * 函数名:iic1_send_ack
 * 函数功能:iic1接收应答信号
 * 函数参数：无
 * 函数返回值：u8--接收应答信号
 * 函数说明：
 ***************************************************/
void iic1_send_ack(u8 ack)
{
    //拉低时钟线,准备应答信号
    IIC1_SCK_L;
    delay_us(IIC1_DELAY);//延时
    if(ack)
        IIC1_SDA_H;
    else
        IIC1_SDA_L;
    delay_us(IIC1_DELAY);//延时
    //拉高时钟线,发送应答信号
    IIC1_SCK_H;
    delay_us(IIC1_DELAY);//延时

    //拉低时钟线，保证周期完整
    IIC1_SCK_L;
    delay_us(IIC1_DELAY);//延时
}

/**************************************************
 * 函数名:iic1_rec_data
 * 函数功能:iic1接收数据
 * 函数参数：无
 * 函数返回值：u8--接收数据
 * 函数说明：
 ***************************************************/
u8 iic1_rec_data(void)
{
    u8 data = 0;
    //拉高数据线，切换输入模式
    IIC1_SDA_H;
    for(int i = 0;i<8;i++)
    {   
        IIC1_SCK_L;//拉低时钟线，准备接收数据
        delay_us(IIC1_DELAY);//延时
        IIC1_SCK_H;//拉高时钟线，接收数据
        delay_us(IIC1_DELAY);//延时
        data <<= 1;
        if(IIC1_SDA_IN)
            data |= 1;  
        delay_us(IIC1_DELAY);//延时
    }
    return data;
}


/**************************************************
 * 函数名:iic1_send_data
 * 函数功能:iic1发送数据
 * 函数参数：u8 data--要发送的数据
 * 函数返回值：无
 * 函数说明：
 ***************************************************/
void iic1_send_data(u8 data)
{
    for(int i = 0;i<8;i++)
    {
        //拉低时钟线，准备发送数据
        IIC1_SCK_L;
        delay_us(IIC1_DELAY);//延时
        if(data & (0x80 >> i))
            IIC1_SDA_H;
        else
            IIC1_SDA_L;
        delay_us(IIC1_DELAY);//延时
        //拉高时钟线,发送数据
        IIC1_SCK_H;
        delay_us(IIC1_DELAY);//延时
    }
}
