#ifndef __DMA_H__
#define __DMA_H__
#include "main.h"

//宏定义


//函数声明
void dma_init(void);
void dma_send_data(u8 *data, u32 size);

#endif