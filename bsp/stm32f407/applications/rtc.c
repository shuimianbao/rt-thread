#include <rtthread.h>
#include "stm32f4xx.h"
#include "rtc.h"

rt_thread_t rtc_thread;
void RTC_TimeShow(void)
{
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
  /* Get the current Time */
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
  /* Display time Format : hh:mm:ss */
	rt_kprintf("%0.4d:%0.2d:%0.2d\n",RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);
  rt_kprintf("%0.2d:%0.2d:%0.2d\n",RTC_TimeStructure.RTC_Hours, RTC_TimeStructure.RTC_Minutes, RTC_TimeStructure.RTC_Seconds);
}

static void RTC_Config(void)
{
	__IO uint32_t uwAsynchPrediv = 0;
	__IO uint32_t uwSynchPrediv = 0;
	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_InitTypeDef  RTC_InitStructure;
  
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {  
    /* RTC configuration  */
    /* Enable the PWR clock */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

		/* Allow access to RTC */
		PWR_BackupAccessCmd(ENABLE);

		/* Enable the LSE OSC */
		RCC_LSEConfig(RCC_LSE_ON);

		/* Wait till LSE is ready */  
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
		{
		}

		/* Select the RTC Clock Source */
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		/* ck_spre(1Hz) = RTCCLK(LSE) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
		uwSynchPrediv = 0xFF;
		uwAsynchPrediv = 0x7F;

		
		/* Enable the RTC Clock */
		RCC_RTCCLKCmd(ENABLE);

		/* Wait for RTC APB registers synchronisation */
		RTC_WaitForSynchro();
		
		/* Configure the RTC data register and RTC prescaler */
		RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;
		RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
		RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
		RTC_Init(&RTC_InitStructure);
		
	 
		/* Set the date: Friday January 11th 2013 */
		RTC_DateStructure.RTC_Year = 0x18;
		RTC_DateStructure.RTC_Month = RTC_Month_December;
		RTC_DateStructure.RTC_Date = 0x24;
		RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Saturday;
		RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		
		/* Set the time to 05h 20mn 00s AM */
		RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
		RTC_TimeStructure.RTC_Hours   = 0x22;
		RTC_TimeStructure.RTC_Minutes = 0x20;
		RTC_TimeStructure.RTC_Seconds = 0x00; 
		
		RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
		
		/* Indicator for the RTC configuration */
		RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
    /* Display the RTC Time and Alarm */
		rt_kprintf("Init RTC\n");
    RTC_TimeShow();

  }
  else
  { 
    
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Display the RTC Time and Alarm */
		rt_kprintf("RTC is inited\n");
    RTC_TimeShow();

  }
}

void rtc_thread_entry(void* parameter)
{
	RTC_Config();

	while(1)
	{
		rt_thread_delay(RT_TICK_PER_SECOND*10);
		//RTC_TimeShow();
	}
}

void RTCSetTime(RTC_TIME_BIN newtime)
{
	;
}
void RTCSetDate(RTC_DATE_BIN newdate)
{
	;
}
