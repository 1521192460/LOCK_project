#ifndef __CY8CMBR3116_H__
#define __CY8CMBR3116_H__
#include "main.h"
//宏定义
//IIC1_RES
#define CY8CMBR3116_RES_H (GPIO_SetBits(GPIOA, GPIO_Pin_1))
#define CY8CMBR3116_RES_L (GPIO_ResetBits(GPIOA, GPIO_Pin_1))


#define CY8CMBR3116_ADDR_WRITE 0X6E
#define CY8CMBR3116_ADDR_READ 0X6F
//函数声明
void CY8CMBR3116_init(void);
u8 CY8CMBR3116_get_key(void);
u8 CY8CMBR3116_key_scan(void);  
#endif