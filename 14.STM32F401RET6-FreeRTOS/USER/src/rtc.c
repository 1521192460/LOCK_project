#include "rtc.h"



/****************************************
 * 函数名：set_time
 * 函数功能：设置时间
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * ****************************************/
void set_time(RTC_t time)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_TimeStruct.RTC_Hours = time.hour;
    RTC_TimeStruct.RTC_Minutes = time.min;
    RTC_TimeStruct.RTC_Seconds = time.sec;
    RTC_SetTime(RTC_Format_BIN, &RTC_TimeStruct);

}


/****************************************
 * 函数名：set_date
 * 函数功能：设置日期
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * ****************************************/
void set_date(RTC_t time)
{
    RTC_DateTypeDef RTC_DateStruct = {0};
    RTC_DateStruct.RTC_Year = time.year - 2000;
    RTC_DateStruct.RTC_Month = time.mon;
    RTC_DateStruct.RTC_Date = time.day;
    RTC_SetDate(RTC_Format_BIN, &RTC_DateStruct);
}


/****************************************
 * 函数名：clock_init
 * 函数功能：初始化RTC
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * ****************************************/
void clock_init(void)
{       
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
    //解除写使能---通过电源控制解除内部时钟写禁止
    PWR_BackupAccessCmd(ENABLE);
    //时钟使能
    RCC_LSEConfig(RCC_LSE_ON);             //外部低速时钟使能
    while(!RCC_GetFlagStatus(RCC_FLAG_LSERDY));//等待外部低速时钟就绪
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择外部低速时钟作为RTC时钟源
    RCC_RTCCLKCmd(ENABLE);//使能RTC时钟

    //解除写保护
    RTC_WriteProtectionCmd(DISABLE);
    //初始化RTC
    RTC_InitTypeDef RTC_InitStruct = {0};
    RTC_InitStruct.RTC_AsynchPrediv = 128 - 1;         //异步预分频--128 32768/128 = 256Hz   默认值
    RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;  //24小时制  
    RTC_InitStruct.RTC_SynchPrediv = 256 - 1;        //同步预分频--256 256/256 = 1Hz  默认值
    RTC_Init(&RTC_InitStruct);
}






/****************************************
 * 函数名：get_time_date
 * 函数功能：获取时间和日期
 * 函数参数：无
 * 函数返回值：无
 * 函数说明：
 * ****************************************/
RTC_t get_time_date(void)
{
    RTC_t time = {0};

    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    RTC_GetTime(RTC_Format_BIN, &RTC_TimeStruct);
    RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

    time.hour = RTC_TimeStruct.RTC_Hours;
    time.min = RTC_TimeStruct.RTC_Minutes;
    time.sec = RTC_TimeStruct.RTC_Seconds;

    return time;
}
