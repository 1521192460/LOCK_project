#ifndef __AT24C02_H__
#define __AT24C02_H__
#include "main.h"

//宏定义
#define AT24C02_ADDR_WRITE 0xA0
#define AT24C02_ADDR_READ 0xA1
#define AT24C02_ACK 0x00
#define AT24C02_NACK 0x01



//函数声明
void at24c02_init(void);
u8 at24c02_write_byte(u8 addr,u8 data);
u8 at24c02_read_byte(u8 addr,u8 *data);
void at24c02_write_page(u8 addr,u8 size,u8 *data);
void at24c02_sequential_read(u8 addr,u8 size,u8 *data);
void at24c02_write_cross_page(u8 addr,u16 size,u8 *data);

#endif