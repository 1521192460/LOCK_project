#ifndef __DMA_H__
#define __DMA_H__
#include "main.h"

//宏定义


//函数声明
void dma1_init(void);
void dma1_spi2_tx(u8 *buf, u16 len);


#endif