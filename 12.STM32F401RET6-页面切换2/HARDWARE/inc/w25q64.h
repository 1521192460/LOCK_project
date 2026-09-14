#ifndef __W25Q64_H__
#define __W25Q64_H__
#include "main.h"
//宏定义
#define W25Q64_CS_H (GPIO_SetBits(GPIOA, GPIO_Pin_4))
#define W25Q64_CS_L (GPIO_ResetBits(GPIOA, GPIO_Pin_4))

//函数声明
void w25q64_init(void);
void w25q64_read_id(void);
void w25q64_write_enable(void);
u8 w25q64_read_busy(void);
void w25q64_sector_erase(u32 addr);
void w25q64_write_page(u32 addr,u8 data_size,u8 *data);
void w25q64_cross_page_write(u32 addr,u16 data_size,u8 *data);
void w25q64_read_data(u32 addr,u8 data_size,u8 *data);
void w25q64_block_erase(u32 addr);
#endif