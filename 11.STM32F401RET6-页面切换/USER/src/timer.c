#include "timer.h"


/*******************************
 * 函数名：timer9_delay_ms
 * 函数功能：定时器9毫秒级延时
 * 函数参数：ms
 * 返回值：无
 * 函数说明：使用定时器9实现毫秒级延时
 ******************************/
void timer9_delay_ms(u16 ms)
{
    //时钟使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);
    //时基单元配置
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 8400 - 1;          
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period = ms * 10 - 1;
    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);
    //清除更新标志，防止轮询立即退出
    TIM_ClearFlag(TIM9, TIM_FLAG_Update);
    //使能定时器9
    TIM_Cmd(TIM9, ENABLE);
    //等待延时时间到
    while (TIM_GetFlagStatus(TIM9, TIM_FLAG_Update) == RESET);
    //关闭定时器9
    TIM_Cmd(TIM9, DISABLE);
}