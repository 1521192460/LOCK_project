#ifndef __WIFI_H__
#define __WIFI_H__
#include "main.h"
//宏定义


//函数声明
void WIFI_init(void);
void WIFI_ctrl(void);
void WIFI_report_password(void);
u8 WIFI_send_data(u8 *cmd,u16 timeout);

#endif