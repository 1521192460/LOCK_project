#ifndef __IIC_H__
#define __IIC_H__
#include "main.h"

/***************************************AT24C02 I2C********************************/
//宏定义
#define IIC3_SCK_H (GPIO_SetBits(GPIOA, GPIO_Pin_8))
#define IIC3_SCK_L (GPIO_ResetBits(GPIOA, GPIO_Pin_8))
#define IIC3_SDA_H (GPIO_SetBits(GPIOC, GPIO_Pin_9))
#define IIC3_SDA_L (GPIO_ResetBits(GPIOC, GPIO_Pin_9))
#define IIC3_SDA_IN (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9))
#define IIC3_DELAY 3

//函数声明
void iic3_pin_init(void);
void iic3_start(void);
void iic3_stop(void);
void iic3_send_data(u8 data);
u8 iic3_rec_data(void);
u8 iic3_rec_ack(void);
void iic3_send_ack(u8 ack);
/**************************************CY8CMBR3116 I2C********************************/
//宏定义
#define IIC1_SCK_H (GPIO_SetBits(GPIOB, GPIO_Pin_6))
#define IIC1_SCK_L (GPIO_ResetBits(GPIOB, GPIO_Pin_6))
#define IIC1_SDA_H (GPIO_SetBits(GPIOB, GPIO_Pin_7))
#define IIC1_SDA_L (GPIO_ResetBits(GPIOB, GPIO_Pin_7))
#define IIC1_SDA_IN (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))
#define IIC1_DELAY 6

//函数声明
void iic1_pin_init(void);
void iic1_start(void);
void iic1_stop(void);
u8 iic1_rec_ack(void);
void iic1_send_ack(u8 ack);
u8 iic1_rec_data(void);
void iic1_send_data(u8 data);
#endif