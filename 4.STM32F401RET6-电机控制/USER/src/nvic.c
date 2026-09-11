#include "nvic.h"



/*************************************
 * 函数名：USART1_IRQHandler
 * 函数功能：串口1中断服务函数
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *************************************/
U1 u1 = {0};
void USART1_IRQHandler(void)
{
    //如果是接收中断
    if(USART_GetITStatus(USART1,USART_IT_RXNE))
    {
        //清除中断标志位
        USART1->DR;
        //接收数据
        u1.buff[u1.len++] = USART_ReceiveData(USART1);
    }
    //如果是空闲中断
    if(USART_GetITStatus(USART1,USART_IT_IDLE))
    {
        //清除中断标志位
        USART1->SR;
        USART1->DR;
        //接收最后一位补'\0'
        u1.buff[u1.len] = '\0';
        //清空接收数据缓冲区
        u1.len = 0;
        //中断标志位置1
        u1.flag = 1;
    }
}
