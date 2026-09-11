#ifndef _USART_H
#define _USART_H
#include "main.h"

//宏定义

//函数声明
void usart1_init(u32 bauds);
void usart1_send_byte(u8 byte);
void usart1_send_str(u8 *str);
#endif