#ifndef __SPI_H__
#define __SPI_H__
#include "main.h"
//宏定义



//函数声明
void w25q64_pin_init(void);
u8 w25q64_transfer_data(u8 data);
#endif