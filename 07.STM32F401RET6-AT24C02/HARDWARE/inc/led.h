#ifndef _LED_H_
#define _LED_H_
#include "main.h"
//宏定义
#define LEDR_ON         (GPIO_ResetBits(GPIOB, GPIO_Pin_8))
#define LEDR_OFF        (GPIO_SetBits(GPIOB, GPIO_Pin_8))
#define LEDR_TOGGLE     (GPIO_ToggleBits(GPIOB, GPIO_Pin_8))

#define LEDG_ON         (GPIO_ResetBits(GPIOB, GPIO_Pin_9))
#define LEDG_OFF        (GPIO_SetBits(GPIOB, GPIO_Pin_9))
#define LEDG_TOGGLE     (GPIO_ToggleBits(GPIOB, GPIO_Pin_9))
//函数声明
void led_init(void);
#endif

