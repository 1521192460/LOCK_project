#ifndef __NV400F_H__
#define __NV400F_H__
#include "main.h"
//宏定义
#define VOICE_DATA_L (GPIO_ResetBits(GPIOC, GPIO_Pin_5))
#define VOICE_DATA_H (GPIO_SetBits(GPIOC, GPIO_Pin_5))

#define VOICE_BUSY (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4))
//函数声明
void NV400F_send_data(u8 cmd);
void NV400F_init(void);

#endif
