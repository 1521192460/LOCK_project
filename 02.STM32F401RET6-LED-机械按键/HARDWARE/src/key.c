#include "key.h"


/***************
 * 函数名：key_init
 * 函数功能：初始化按键
 * 函数参数：无
 * 函数返回值：无
 * 函数说明
 *         KEY1---WKUP----PA0
 *         无上下拉电阻、输入模式、检测高电平为按下
 *         KEY2---BACKUP--PC13
 *         无上下拉电阻、输入模式、检测低电平为按下
 ***************/
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;          //输入模式


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;            //KEY2引脚，按下为低电平，默认高电平
    GPIO_Init(GPIOC, &GPIO_InitStructure);                //初始化GPIOC13

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;             //KEY1引脚，按下为高电平，默认低电平
    GPIO_Init(GPIOA, &GPIO_InitStructure);                //初始化GPIOA0
}

/**********************
 * 函数名：key_scan
 * 函数功能：按键扫描
 * 函数参数：无
 * 函数返回值：u8---按键值
 * 函数说明
 **********************/
u8 key_scan(void)
{
    //设置标志位，判断按键按下
    static u8 key_flag = 1;
    if((KEY2 || KEY1) && key_flag)
    {
        key_flag = 0; //按键按下，清除标志位
        delay_ms(12); //消抖
        if(KEY2 || KEY1) //再次判断按键是否按下
        {
            //判断是按键1
            if(KEY1)
            {
                return 1;
            }
            //判断是按键2
            else if(KEY2)
            {
                return 2;
            }
        }
    }
    //按键松开，重置标志位标志位
    else if((!KEY2 && !KEY1) && !key_flag)
    { 
        key_flag = 1;
    }
    return 0;
}
