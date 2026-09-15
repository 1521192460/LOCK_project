#ifndef _NVIC_H
#define _NVIC_H
#include "main.h"

//串口1接收数据结构体定义
typedef struct
{
    u8  buff[512];    //接收数据缓冲区
    u16 len;         //记录接收数据的长度
    u8  flag;        //接收数据完成标志位
}USART_STATUE;

extern USART_STATUE u1;
extern USART_STATUE u6;
extern USART_STATUE u2;

//宏定义

//函数声明
#endif

