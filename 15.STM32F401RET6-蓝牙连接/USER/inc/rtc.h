#ifndef __RTC_H__
#define __RTC_H__
#include "main.h"
typedef struct rtc
{
	u16 year;
	u8 mon;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	
}RTC_t;
//宏定义

//函数声明
void clock_init(void);
RTC_t get_time_date(void);
void set_time(RTC_t time);
void set_date(RTC_t date);
#endif
