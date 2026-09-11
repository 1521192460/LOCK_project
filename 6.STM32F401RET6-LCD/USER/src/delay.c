#include "delay.h"

/*************************************
 * 函数名：delay_us
 * 函数功能：延时微秒
 * 函数参数：u32 us---延时的微秒数
 * 函数返回值：无
 * 函数说明：
 *           84MHz时钟下，延时1微秒需要1个时钟周期，1个时钟周期需要1个时钟周期
 ************************************/
void delay_us(u32 us)
{
    while(us--)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();
    }
}

/*************************************
 * 函数名：delay_ms
 * 函数功能：延时毫秒
 * 函数参数：u32 ms---延时的毫秒数
 * 函数返回值：无
 * 函数说明：
 ************************************/
void delay_ms(u32 ms)
{
    while(ms--)
    {
        delay_us(1000);
    }
}
