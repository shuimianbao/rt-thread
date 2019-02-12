#include "stm32f2xx.h"
#include "system.h"
#include "spi1.h"
#include "glcd.h"
#include "cmd_a.h"
#include "usart1.h"




int main(void)
{
	unsigned char rtc_tab[7];
	
	//STM32
	HAL_Init();
	SYS_clock_config();
	USART1_init();
	SPI1_init();

	printf("www.neqee.com\n");

	//GLCD
	GLCD_wait_power_on();
	printf("GLCD init done.\n");
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
	__HAL_RCC_CRC_CLK_ENABLE();//emwin encrypt
	MainTask();
}
