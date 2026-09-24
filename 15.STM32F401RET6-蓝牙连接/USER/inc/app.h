#ifndef _APP_H
#define _APP_H
#include "main.h"
extern TaskHandle_t start_task_handle;
extern TaskHandle_t open_page_task_handle;
extern TaskHandle_t admin_check_task_handle;
extern TaskHandle_t admin_page_task_handle;
//宏定义


//函数声明
void start_task(void *ptr);
void timer1_callback(void);
void timer2_callback(void);


#endif
