#ifndef _RTC_H_
#define _RTC_H_

#include <rtthread.h>
#include <stm32f4xx.h>

typedef struct{
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} RTC_TIME_BIN;

typedef struct{
	uint8_t year;  //without 20xx
	uint8_t month;
	uint8_t day;
} RTC_DATE_BIN;

extern rt_thread_t rtc_thread;


static void RTC_Config(void);
void rtc_thread_entry(void* parameter);
void RTC_TimeShow(void);
void RTCSetTime(RTC_TIME_BIN newtime);
void RTCSetDate(RTC_DATE_BIN newdate);
#endif

