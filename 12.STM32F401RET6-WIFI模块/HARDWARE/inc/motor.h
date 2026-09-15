#ifndef __MOTOR_H_
#define __MOTOR_H_
#include "main.h"

//宏定义

#define MOTOR_IB_H (GPIO_SetBits(GPIOB, GPIO_Pin_3))
#define MOTOR_IB_L (GPIO_ResetBits(GPIOB, GPIO_Pin_3))

#define MOTOR_IA_H (GPIO_SetBits(GPIOB, GPIO_Pin_4))
#define MOTOR_IA_L (GPIO_ResetBits(GPIOB, GPIO_Pin_4))

#define LOCK_ON {motor_ctrl(1000);delay_ms(300);motor_ctrl(0);}
#define LOCK_OFF {motor_ctrl(-1000);delay_ms(300);motor_ctrl(0);}



//函数声明
void motor_init(void);
void motor_ctrl(s16 speed);
#endif