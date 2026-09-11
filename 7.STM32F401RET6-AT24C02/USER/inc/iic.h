#ifndef __IIC_H__
#define __IIC_H__
#include "main.h"
//宏定义
//时钟输出
#define IIC3_SCK_H (GPIO_SetBits(GPIOA, GPIO_Pin_8))
#define IIC3_SCK_L (GPIO_ResetBits(GPIOA, GPIO_Pin_8))

//数据输出
#define IIC3_SDA_H (GPIO_SetBits(GPIOC, GPIO_Pin_9))
#define IIC3_SDA_L (GPIO_ResetBits(GPIOC, GPIO_Pin_9))

//数据输入
#define IIC3_SDA_IN (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))

//iic3延时时间
#define IIC3_DELAY 6

//函数声明
void iic3_pin_init(void);
void iic3_start(void);
void iic3_stop(void);
void iic3_send_data(u8 data);
u8 iic3_rec_data(void);
u8 iic3_rec_ack(void);
void iic3_send_ack(u8 ack); 
#endif