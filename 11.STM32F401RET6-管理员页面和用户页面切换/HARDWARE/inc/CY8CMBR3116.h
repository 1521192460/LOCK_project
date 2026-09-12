#ifndef __CY8CMBR3116_H__
#define __CY8CMBR3116_H__
#include "main.h"
//宏定义
//IIC1_RES
#define CY8CMBR3116_RES_H (GPIO_SetBits(GPIOA, GPIO_Pin_1))
#define CY8CMBR3116_RES_L (GPIO_ResetBits(GPIOA, GPIO_Pin_1))


#define CY8CMBR3116_ADDR_WRITE 0X6E
#define CY8CMBR3116_ADDR_READ 0X6F



#define AUTH_NONE       0
#define AUTH_USER_OK    1
#define AUTH_ADMIN_OK   2
#define AUTH_FAILED     3
//函数声明
void CY8CMBR3116_init(void);
u8 CY8CMBR3116_get_key(void);
u8 CY8CMBR3116_key_scan(void);  
void check_init_password(void);
void set_admin_password(void);
u8 password_open_door(void);
#endif