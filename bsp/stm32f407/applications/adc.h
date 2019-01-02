#ifndef _ADC_H_
#define _ADC_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define ADCx											ADC1
#define ADCx_CLK									RCC_APB2Periph_ADC1
#define ADCx_CHANNEL              ADC_Channel_9
#define ADCx_CHANNEL_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define ADCx_GPIO_PIN							GPIO_Pin_1
#define ADCx_GPIO_PORT						GPIOB
 
extern rt_thread_t adc_thread;

static void ADC_Config(void);
void ShowADCResult(void);
void adc_thread_entry(void* parameter);
#endif
