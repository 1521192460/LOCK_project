#ifndef _USART_H
#define _USART_H
#include "main.h"

//宏定义

//函数声明
void usart1_init(u32 bauds);
void usart1_send_byte(u8 byte);
void usart1_send_str(u8 *str);

void usart6_init(u32 bauds);
void usart6_send_byte(u8 byte);
void usart6_send_str(u8 *str);


void usart2_init(u32 bauds);
void usart2_send_byte(u8 byte);
void usart2_send_str(u8 *str);

#endif