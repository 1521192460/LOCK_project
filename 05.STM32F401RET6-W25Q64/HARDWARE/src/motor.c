#include "motor.h"


/***************************
 * 函数名：motor_init
 * 函数功能：电机初始化
 * 函数参数：无
 * 函数返回值：无
 * 函数声明：
 *          MOTOR-IB----PB3---复用推挽输出、无上下拉、50MHz、TIM2_CH2
 *          MOTOR-IA----PB4---复用推挽输出、无上下拉、50MHz、TIM3_CH1
 ***************************/
void motor_init(void)
{
    //时钟使能
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure = {0};              
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           //复用模式
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;        //无上下拉
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;        //50MHz
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_TIM3);
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*定时器2通道2输出比较 */
    //时基单元初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; //向上计数
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;              //ARR
    TIM_TimeBaseInitStruct.TIM_Prescaler = 84 - 1;             //PSC
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
    //通道2初始化
    TIM_OCInitTypeDef TIM_OCInitStruct = {0};
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; //PWM1模式
    TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; //高电平有效
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //使能输出
    TIM_OCInitStruct.TIM_Pulse = 0; //占空比为0
    TIM_OC2Init(TIM2, &TIM_OCInitStruct);

    /*定时器3通道1输出比较 */
    //通道1初始化
    TIM_OC1Init(TIM3, &TIM_OCInitStruct);

    //使能定时器2
    TIM_Cmd(TIM2, ENABLE);
    //使能定时器3
    TIM_Cmd(TIM3, ENABLE);
}


/***************************
 * 函数名：motor_ctrl
 * 函数功能：电机控制
 * 函数参数：s16 speed -1000~1000，0为停止，正为正转，负为反转
 * 函数返回值：无
 * 函数声明： *          
 *          MOTOR-IB----PB3---TIM2_CH2
 *          MOTOR-IA----PB4---TIM3_CH1
 ***************************/
void motor_ctrl(s16 speed)
{
    if(speed == 0)
    {
        TIM_SetCompare1(TIM3, 0);
        TIM_SetCompare2(TIM2, 0);
    }
    else if(speed > 0)
    {
        TIM_SetCompare1(TIM3, speed);
        TIM_SetCompare2(TIM2, 0);
    }
    else if(speed < 0)
    {
        TIM_SetCompare1(TIM3, 0);
        TIM_SetCompare2(TIM2, -speed);
    }

}
