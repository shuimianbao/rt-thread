#include <rtthread.h>
#include "stm32f4xx.h"
#include "extio.h"

rt_thread_t extio_thread;

static void Ext_Out_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(EXT_OUT1_GPIO_CLK | EXT_OUT6_GPIO_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		
	GPIO_InitStructure.GPIO_Pin = EXT_OUT1_PIN | EXT_OUT2_PIN | EXT_OUT3_PIN | EXT_OUT4_PIN | EXT_OUT5_PIN;
  GPIO_Init(EXT_OUT1_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = EXT_OUT6_PIN | EXT_OUT7_PIN | EXT_OUT8_PIN;
  GPIO_Init(EXT_OUT6_PORT, &GPIO_InitStructure);
}
void Ext_Out_ResetBits(uint16_t GPIO_Pin)
{
	GPIO_SetBits(EXT_OUT1_PORT, GPIO_Pin & 0x1FFF);
	GPIO_SetBits(EXT_OUT6_PORT, GPIO_Pin & 0xE000);
}
void Ext_Out_SetBits(uint16_t GPIO_Pin)
{
	GPIO_ResetBits(EXT_OUT1_PORT, GPIO_Pin & 0x1FFF);
	GPIO_ResetBits(EXT_OUT6_PORT, GPIO_Pin & 0xE000);
}

void extio_thread_entry(void* parameter)
{
	Ext_Out_Config();
	rt_kprintf("extio_thread_entry running\n");

	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		//ShowADCResult();
		Ext_Out_SetBits(EXT_OUT1_PIN | EXT_OUT2_PIN | EXT_OUT3_PIN | EXT_OUT4_PIN | \
										EXT_OUT5_PIN | EXT_OUT6_PIN | EXT_OUT7_PIN | EXT_OUT8_PIN);
	}
}
