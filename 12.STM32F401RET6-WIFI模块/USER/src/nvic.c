#include "nvic.h"



/*************************************
 * 函数名：USART1_IRQHandler
 * 函数功能：串口1中断服务函数
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *************************************/
USART_STATUE u1 = {0};
void USART1_IRQHandler(void)
{
    static u32 zk_addr = 0;
    //如果是接收中断
    if(USART_GetITStatus(USART1,USART_IT_RXNE))
    {
        //清除中断标志位
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        /***********************************接收数据使用**************************************************/
        //接收数据
        // u1.buff[u1.len++] = USART_ReceiveData(USART1);
        
        /***********************************烧录字库文件使用**************************************************/
        file_flag = 1;//已发送文件
        u8 data = USART_ReceiveData(USART1);  // 接收数据
        w25q64_write_page(zk_addr, 1, &data); 
        zk_addr++;
    }
    //如果是空闲中断
    if(USART_GetITStatus(USART1,USART_IT_IDLE))
    {
        //清除中断标志位
        USART1->SR;
        USART1->DR;
        /***********************************接收数据使用**************************************************/
        // 接收最后一位补'\0'
        // u1.buff[u1.len] = '\0';
        // 清空接收数据缓冲区
        // u1.len = 0;
        // 中断标志位置1
        // u1.flag = 1;
        /***********************************烧录字库使用**************************************************/
        //烧录完成标志位
        if(zk_addr >= 0x001D3374)//当写入到字库文件结束地址时，烧录完成
        {
            update_flag = 0;
            file_flag = 0;
        }
    }
}





/*************************************
 * 函数名：USART6_IRQHandler
 * 函数功能：串口6中断服务函数
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *************************************/
USART_STATUE u6 = {0};
void USART6_IRQHandler(void)
{
    //如果是接收中断
    if(USART_GetITStatus(USART6,USART_IT_RXNE))
    {
        //清除中断标志位
        USART_ClearITPendingBit(USART6, USART_IT_RXNE);
        //接收数据
         u6.buff[u6.len++] = USART_ReceiveData(USART6);
        
    }
    //如果是空闲中断
    if(USART_GetITStatus(USART6,USART_IT_IDLE))
    {
        //清除中断标志位
        USART6->SR;
        USART6->DR;
        
        // 接收最后一位补'\0'
        u6.buff[u6.len] = '\0';
        // 清空接收数据缓冲区
        u6.len = 0;
        // 中断标志位置1
        u6.flag = 1;

    }
}





/*************************************
 * 函数名：USART2_IRQHandler
 * 函数功能：串口2中断服务函数
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 *************************************/
USART_STATUE u2 = {0};
void USART2_IRQHandler(void)
{
    //如果是接收中断
    if(USART_GetITStatus(USART2,USART_IT_RXNE))
    {
        //清除中断标志位
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        //接收数据
         u2.buff[u2.len++] = USART_ReceiveData(USART2);
        
    }
    //如果是空闲中断
    if(USART_GetITStatus(USART2,USART_IT_IDLE))
    {
        //清除中断标志位
        USART2->SR;
        USART2->DR;
        
        // 接收最后一位补'\0'
        u2.buff[u2.len] = '\0';
        // 清空接收数据缓冲区
        u2.len = 0;
        // 中断标志位置1
        u2.flag = 1;
        printf("%s",u2.buff);

    }
}