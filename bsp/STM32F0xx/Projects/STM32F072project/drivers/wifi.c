#include <rtthread.h>
#include <stdlib.h>
#include <stm32f0xx.h>
#include "usart.h"
#include "wifi.h"
#include "finsh.h"

/* wifi using USART2 */
#define UART2_GPIO_TX			GPIO_Pin_2
#define UART2_GPIO_TX_SOURCE	GPIO_PinSource2
#define UART2_GPIO_RX			GPIO_Pin_3
#define UART2_GPIO_RX_SOURCE	GPIO_PinSource3
#define UART2_GPIO_AF			GPIO_AF_1
#define UART2_GPIO				GPIOA
#define WIFI_IRQn                      USART2_IRQn
#define WIFI_IRQHandler                USART2_IRQHandler

static uint8_t recch;
rt_thread_t wifi_thread;
void init_wifi(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
	   /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    /* Enable USART clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* Connect PXx to USARTx_Tx */
	GPIO_PinAFConfig(UART2_GPIO, UART2_GPIO_TX_SOURCE, UART2_GPIO_AF);

	/* Connect PXx to USARTx_Rx */
	GPIO_PinAFConfig(UART2_GPIO, UART2_GPIO_RX_SOURCE, UART2_GPIO_AF);

	/* Configure USART Tx, Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UART2_GPIO_TX | UART2_GPIO_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(UART2_GPIO, &GPIO_InitStructure);

	/* USARTx configuration ----------------------------------------------------*/
  	/* USARTx configured as follow:
  	- BaudRate = 115200 baud  
 	 - Word Length = 8 Bits
  	- One Stop Bit
  	- No parity
 	 - Hardware flow control disabled (RTS and CTS signals)
 	 - Receive and transmit enabled
  	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
  
  /* NVIC configuration */
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = WIFI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  /* Enable USART */
  USART_Cmd(USART2, ENABLE);

}

void wifi_thread_entry(void* parameter)
{
	init_wifi();
	
	while(1)
	{

        //rt_thread_delay(RT_TICK_PER_SECOND);
        rt_thread_suspend(wifi_thread);
		rt_schedule();
		USART_SendData(USART2, recch);

	}
}

void WIFI_IRQHandler(void)
{
  /* USART in mode Transmitter -------------------------------------------------*/
  if (USART_GetITStatus(USART2, USART_IT_TXE) == SET)
  { 
  }
  
  /* USART in mode Receiver --------------------------------------------------*/
  if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
  {
  	recch = USART_ReceiveData(USART2);
	rt_thread_resume(wifi_thread);
    
  }     
}