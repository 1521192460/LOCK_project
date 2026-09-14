#include "led.h"



/*************************
 * 函数名：led_init
 * 函数功能：初始化LED
 * 函数参数：无
 * 函数返回值：无
 * 函数说明
        PB8---LEDR---通用推挽输出
        PB9---LEDG---通用推挽输出
*************************/
void led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //LED引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;          //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;        //50MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       //无上下拉电阻
    GPIO_Init(GPIOB, &GPIO_InitStructure);                 //初始化GPIOB8和GPIOB9

    //设置LED初始状态
    LEDR_OFF;
    LEDG_OFF;
}
