#ifndef __PAGE_H__
#define __PAGE_H__
#include "main.h"
//宏定义
#define USER_PAGE   0
#define ADMIN_PAGE  1


//函数声明
u8 manage_page(void);
u8 user_page(void);


#endif