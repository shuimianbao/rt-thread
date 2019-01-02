#include <rtthread.h>
#include "stm32f4xx.h"
#include "adc.h"

rt_thread_t adc_thread;

static void ADC_Config(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
//DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
    
  /* Enable peripheral clocks *************************************************/
  //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_AHB1PeriphClockCmd(ADCx_CHANNEL_GPIO_CLK, ENABLE); 
  RCC_APB2PeriphClockCmd(ADCx_CLK, ENABLE);
   
	/* Configure ADC3 Channel7 pin as analog input ******************************/
  GPIO_InitStructure.GPIO_Pin = ADCx_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(ADCx_GPIO_PORT, &GPIO_InitStructure);	
	
  /* ADC Common Init **********************************************************/
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC1 Init ****************************************************************/
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//ENABLE;//
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADCx, &ADC_InitStructure);

   
  /* ADC1 regular channel9 configuration ******************************/
  ADC_RegularChannelConfig(ADCx, ADCx_CHANNEL, 1, ADC_SampleTime_15Cycles);

  /* Enable VBAT channel */
  //ADC_VBATCmd(ENABLE); 



  /* Enable ADC1 **************************************************************/
  ADC_Cmd(ADCx, ENABLE);
}

void ShowADCResult(void)
{
	uint16_t adcres;
	/* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADCx);
	while(SET != ADC_GetFlagStatus(ADCx, ADC_FLAG_EOC));
	adcres =ADC_GetConversionValue(ADCx) & 0x0fff;
	//rt_kprintf("ADC result = [%d] %fV\n",adcres, (float)adcres*3.3/(float)0x1000);
	rt_kprintf("ADC result = [%d]\n",adcres);
		
}

void adc_thread_entry(void* parameter)
{
	ADC_Config();

	while(1)
	{
		rt_thread_delay(RT_TICK_PER_SECOND*10);
		ShowADCResult();
	}
}
