#include "usart.h"


/*************************************
 * 函数名：usart1_init
 * 函数功能：串口1初始化
 * 函数参数：u32 bauds
 * 函数返回值：无
 * 函数说明：
 *          PA9--TX--复用推挽输出
 *          PA10--RX--复用输入
 ************************************/
void usart1_init(u32 bauds)
{   //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
    GPIO_Init(GPIOA,&GPIO_InitStruct);

    //串口1初始化
    USART_InitTypeDef USART_InitStruct;
    USART_InitStruct.USART_BaudRate = bauds;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART1,&USART_InitStruct);

    //NVIC初始化
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 7;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    //串口1中断使能---接收中断/空闲中断
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);

    //串口1使能
    USART_Cmd(USART1,ENABLE);

}



/*************************************
 * 函数名：usart1_send_byte
 * 函数功能：串口1发送字节数据
 * 函数参数：u8 byte
 * 函数返回值：无
 * 函数说明：
 ************************************/
void usart1_send_byte(u8 byte)
{
    //等待上一个字节数据发送完成
    while(!USART_GetFlagStatus(USART1,USART_FLAG_TXE));
    //发送字节数据
    USART_SendData(USART1,byte);
}

/*************************************
 * 函数名：usart1_send_str
 * 函数功能：串口1发送字符串数据
 * 函数参数：u8 *str
 * 函数返回值：无
 * 函数说明：
 ************************************/
void usart1_send_str(u8 *str)
{
    while(*str != '\0')
    {
        usart1_send_byte(*str++);
    }
}


/*************************************
 * 函数名：fputc
 * 函数功能：printf重定向函数
 * 函数参数：int ch, FILE *f
 * 函数返回值：无
 * 函数说明：
 ************************************/
int fputc(int c, FILE * stream)
{
    usart1_send_byte(c);
    return c;
}