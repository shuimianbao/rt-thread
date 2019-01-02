#include <rtthread.h>
#include "stm32f4xx.h"
#include "systemled.h"

rt_thread_t systemled_thread;

static int rt_system_led_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    //BELL		PE11  
	//LED_RED PE12
	//LED_GRN PE13
	/* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = BELL | LED_RED |LED_GRN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

	return 0;
}

/* led thread entry */
void systemled_thread_entry(void* parameter)
{
	rt_system_led_init();

	while(1)
	{
		rt_systme_red_led_on();
		rt_systme_grn_led_off();
		rt_thread_delay(RT_TICK_PER_SECOND);

		rt_systme_red_led_off();
		rt_systme_grn_led_on();
		rt_thread_delay(RT_TICK_PER_SECOND);
	}
}
