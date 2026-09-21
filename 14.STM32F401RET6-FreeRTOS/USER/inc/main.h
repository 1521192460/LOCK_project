#ifndef _MAIN_H
#define _MAIN_H

#include "stm32f4xx.h"                 
#include "stdio.h"
#include "string.h"
#include "FreeRTOS.h"

#include "key.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "nvic.h"
#include "motor.h"
#include "timer.h"  
#include "spi.h"
#include "rtc.h"
#include "w25q64.h"
#include "lcd.h"
#include "dma.h"
#include "zk.h"
#include "iic.h"
#include "at24c02.h"
#include "CY8CMBR3116.h"
#include "NV400F.h"
#include "MG200.h"
#include "WIFI.h"
#include "rfid.h"

//FreeRTOS
#include "task.h"
#include "timers.h"
#include "app.h"



extern u8 ui_flag;
#endif
