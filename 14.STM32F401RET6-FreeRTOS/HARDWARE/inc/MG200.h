#ifndef __MG200_H__
#define __MG200_H__ 
#include "main.h"
//宏定义
#define MG200_DETECT    (GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_0))

#define MG200_POWER_ON  GPIO_SetBits(GPIOC,GPIO_Pin_1)
#define MG200_POWER_OFF GPIO_ResetBits(GPIOC,GPIO_Pin_1)

#define MG200_CANCEL  0xEE   // 用户中途按 * 取消

//函数声明
void MG200_init(void);
u8 MG200_get_fingerprint(u8 capture_cnt);
u8 MG200_enroll(u8 id);
u8 MG200_match(void);
u8 MG200_erase(u8 id);
u8 MG200_erase_all(void);
u8 MG200_get_user_num(void);
void MG200_register(void);
void MG200_open_door(void);
void MG200_delete_id(void);
void MG200_delete_all(void);
#endif