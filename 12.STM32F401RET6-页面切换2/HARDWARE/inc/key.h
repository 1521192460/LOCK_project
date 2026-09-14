#ifndef _KEY_H_
#define _KEY_H_
#include "main.h"
//宏定义

#define KEY1  (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
#define KEY2  !(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))

//函数声明
void key_init(void);
u8 key_scan(void);
#endif
 