#ifndef __SYSTEMLED_H__
#define __SYSTEMLED_H__

#include <rtthread.h>
#include <stm32f4xx.h>

#define BELL_GPIO_PORT	GPIOE
#define BELL		GPIO_Pin_11  
#define LED_RED_GPIO_PORT	GPIOE
#define LED_RED  GPIO_Pin_12
#define LED_GRN_GPIO_PORT	GPIOE
#define LED_GRN GPIO_Pin_13

//BELL		PE11 
#define rt_systme_bell_on()		GPIO_SetBits(GPIOE, BELL);
#define rt_systme_bell_off()	GPIO_ResetBits(GPIOE, BELL);
		
		//LED_RED PE12
#define rt_systme_red_led_on()	GPIO_SetBits(LED_RED_GPIO_PORT, LED_RED);
#define rt_systme_red_led_off()	GPIO_ResetBits(LED_RED_GPIO_PORT, LED_RED);
		
		//LED_GRN PE13
#define rt_systme_grn_led_on()		GPIO_SetBits(LED_GRN_GPIO_PORT, LED_GRN);
#define rt_systme_grn_led_off()		GPIO_ResetBits(LED_GRN_GPIO_PORT, LED_GRN);

extern rt_thread_t systemled_thread;

static int rt_system_led_init(void);
void systemled_thread_entry(void* parameter);

#endif
