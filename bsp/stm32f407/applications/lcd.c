#include <rtthread.h>
#include "stm32f4xx.h"
#include "lcd.h"
#include "spi1.h"
#include "gui.h"
#include "glcd.h"
#include "cmd_a.h"

rt_thread_t lcd_thread;
static  rt_timer_t  lcd_os_timer;
extern void MainTask(void);
extern volatile GUI_TIMER_TIME OS_TimeMS;

static void lcd_os_timerout(void*  parameter)
{
	OS_TimeMS++;
}

void lcd_thread_entry(void* parameter)
{
	 unsigned char rtc_tab[7];
	SPI1_init();

	rt_kprintf("www.neqee.com\n");
	lcd_os_timer  =  rt_timer_create("lcd_timer",   //timer name
							lcd_os_timerout,  //  timeout callback function
							RT_NULL,   //callback input para
							1, //time 1 OS Ti=1ms
							RT_TIMER_FLAG_PERIODIC);  //periodic timer
	//start timer
	if(lcd_os_timer  !=  RT_NULL)  
		rt_timer_start(lcd_os_timer);
	
	//GLCD
	GLCD_wait_power_on();
	rt_kprintf("GLCD init done.\n");
	GLCD_system_reset();
	GLCD_bfcolor_set(1, 0xffffff, 0x000000);
	GLCD_trcolor_set(1, 0xf0f0f0);
	//GLCD_fontlib_set(1, 8, 16, "a16:u16");
	CMDA_backlight_setting(12);
	rtc_tab[6]=19; rtc_tab[5]=1; rtc_tab[3]=1; rtc_tab[2]=23; rtc_tab[1]=58; rtc_tab[0]=55; rtc_tab[4]=3;
	CMDA_rtc_setting(&rtc_tab[0]);
	CMDA_rtc_display(0xff0000, 700-26*8-4, 4, 1, 1);
	CMDA_buzzer_beep(60);

	//EMWIN
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//__HAL_RCC_CRC_CLK_ENABLE();//emwin encrypt
	MainTask();

	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		//ShowADCResult();
	}
}


