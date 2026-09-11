#ifndef __SPI_H__
#define __SPI_H__
#include "main.h"
//宏定义

#define LCD_CS_H (GPIO_SetBits(GPIOB,GPIO_Pin_12))
#define LCD_CS_L (GPIO_ResetBits(GPIOB,GPIO_Pin_12))

#define LCD_RESET_H (GPIO_SetBits(GPIOB,GPIO_Pin_10))
#define LCD_RESET_L (GPIO_ResetBits(GPIOB,GPIO_Pin_10))

#define LCD_LEDK_ON (GPIO_SetBits(GPIOB,GPIO_Pin_1))
#define LCD_LEDK_OFF (GPIO_ResetBits(GPIOB,GPIO_Pin_1))

#define LCD_CMD (GPIO_ResetBits(GPIOB,GPIO_Pin_14))
#define LCD_DATA (GPIO_SetBits(GPIOB,GPIO_Pin_14))


//函数声明
void w25q64_pin_init(void);
u8 w25q64_transfer_data(u8 data);
void lcd_pin_init(void);
void lcd_transfer_data(u8 data);
#endif