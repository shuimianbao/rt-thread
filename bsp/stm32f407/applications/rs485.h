#ifndef _RS485_H_
#define _RS485_H_
#include <rtthread.h>
#include "stm32f4xx.h"

typedef enum 
{
  RS485_RECV = 0,
  RS485_SEND = 1
} RS485_DIR;

typedef enum 
{
  RS485_SUBPORT1 = 0,
  RS485_SUBPORT2 = 1,
	RS485_SUBPORT3 = 2,
	RS485_SUBPORT4 = 3
} RS485_SUBPORT;

#define RS485_1                  	USART6      
#define RS485_1_CLK					RCC_APB2Periph_USART6
#define RS485_1_TX_PIN                 GPIO_Pin_6
#define RS485_1_TX_GPIO_PORT           GPIOC
#define RS485_1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define RS485_1_TX_SOURCE              GPIO_PinSource6
#define RS485_1_TX_AF                  GPIO_AF_USART6
#define RS485_1_RX_PIN                 GPIO_Pin_7
#define RS485_1_RX_GPIO_PORT           GPIOC
#define RS485_1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define RS485_1_RX_SOURCE              GPIO_PinSource7
#define RS485_1_RX_AF                  GPIO_AF_USART6
#define RS485_1_IRQn                   USART6_IRQn
#define RS485_1_ISR					USART6_IRQHandler
#define RS485_1_DIR_GPIO_CLK           RCC_AHB1Periph_GPIOD
#define RS485_1_DIR_PIN								 GPIO_Pin_15
#define RS485_1_DIR_PORT							 GPIOD
#define RS485_1_A1_GPIO_CLK           RCC_AHB1Periph_GPIOD
#define RS485_1_A1_PIN								 GPIO_Pin_13									 				
#define RS485_1_A1_PORT							   GPIOD
#define RS485_1_A2_GPIO_CLK           RCC_AHB1Periph_GPIOD
#define RS485_1_A2_PIN								 GPIO_Pin_14
#define RS485_1_A2_PORT							   GPIOD

#define RS485_2                        USART1
#define RS485_2_CLK                    RCC_APB2Periph_USART1
#define RS485_2_TX_PIN                 GPIO_Pin_9
#define RS485_2_TX_GPIO_PORT           GPIOA
#define RS485_2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define RS485_2_TX_SOURCE              GPIO_PinSource9
#define RS485_2_TX_AF                  GPIO_AF_USART1
#define RS485_2_RX_PIN                 GPIO_Pin_10
#define RS485_2_RX_GPIO_PORT           GPIOA
#define RS485_2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define RS485_2_RX_SOURCE              GPIO_PinSource10
#define RS485_2_RX_AF                  GPIO_AF_USART1
#define RS485_2_IRQn                   USART1_IRQn
#define RS485_2_ISR										 USART1_IRQHandler
#define RS485_2_DIR_GPIO_CLK           RCC_AHB1Periph_GPIOC
#define RS485_2_DIR_PIN								 GPIO_Pin_8
#define RS485_2_DIR_PORT							 GPIOC
#define RS485_2_A1_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define RS485_2_A1_PIN								 GPIO_Pin_9									 				
#define RS485_2_A1_PORT							   GPIOC
#define RS485_2_A2_GPIO_CLK           	 RCC_AHB1Periph_GPIOA
#define RS485_2_A2_PIN								 GPIO_Pin_8
#define RS485_2_A2_PORT							   GPIOA


/* Definition for DMAx resources ********************************************/
#define RS485_1_DR_ADDRESS			((uint32_t)USART6 + 0x04) 

#define RS485_1_DMA					DMA2
#define RS485_1_DMA_CLK 				RCC_AHB1Periph_DMA2
   
#define RS485_1_TX_DMA_CHANNEL		DMA_Channel_5
#define RS485_1_TX_DMA_STREAM			DMA2_Stream6
#define RS485_1_TX_DMA_FLAG_FEIF 		DMA_FLAG_FEIF6
#define RS485_1_TX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF6
#define RS485_1_TX_DMA_FLAG_TEIF 		DMA_FLAG_TEIF6
#define RS485_1_TX_DMA_FLAG_HTIF 		DMA_FLAG_HTIF6
#define RS485_1_TX_DMA_FLAG_TCIF 		DMA_FLAG_TCIF6

#define RS485_1_TX_DMA_IT_FEIF 			DMA_FLAG_FEIF6
#define RS485_1_TX_DMA_IT_DMEIF			DMA_IT_DMEIF6
#define RS485_1_TX_DMA_IT_TEIF 			DMA_IT_TEIF6
#define RS485_1_TX_DMA_IT_HTIF 			DMA_IT_HTIF6
#define RS485_1_TX_DMA_IT_TCIF 			DMA_IT_TCIF6

#define RS485_1_RX_DMA_CHANNEL			DMA_Channel_5
#define RS485_1_RX_DMA_STREAM			DMA2_Stream1
#define RS485_1_RX_DMA_FLAG_FEIF 		DMA_FLAG_FEIF1
#define RS485_1_RX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF1
#define RS485_1_RX_DMA_FLAG_TEIF 		DMA_FLAG_TEIF1
#define RS485_1_RX_DMA_FLAG_HTIF 		DMA_FLAG_HTIF1
#define RS485_1_RX_DMA_FLAG_TCIF 		DMA_FLAG_TCIF1

#define RS485_1_RX_DMA_IT_FEIF 			DMA_FLAG_FEIF1
#define RS485_1_RX_DMA_IT_DMEIF			DMA_IT_DMEIF1
#define RS485_1_RX_DMA_IT_TEIF 			DMA_IT_TEIF1
#define RS485_1_RX_DMA_IT_HTIF 			DMA_IT_HTIF1
#define RS485_1_RX_DMA_IT_TCIF 			DMA_IT_TCIF1

#define RS485_1_DMA_TX_IRQn				DMA2_Stream6_IRQn
#define RS485_1_DMA_RX_IRQn				DMA2_Stream1_IRQn
#define RS485_1_DMA_TX_IRQHandler		DMA2_Stream6_IRQHandler
#define RS485_1_DMA_RX_IRQHandler		DMA2_Stream1_IRQHandler


/* Definition for DMAx resources ********************************************/
#define RS485_2_DR_ADDRESS				((uint32_t)USART1 + 0x04) 

#define RS485_2_DMA						DMA2
#define RS485_2_DMA_CLK 				RCC_AHB1Periph_DMA2
   
#define RS485_2_TX_DMA_CHANNEL			DMA_Channel_4
#define RS485_2_TX_DMA_STREAM			DMA2_Stream7
#define RS485_2_TX_DMA_FLAG_FEIF 		DMA_FLAG_FEIF7
#define RS485_2_TX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF7
#define RS485_2_TX_DMA_FLAG_TEIF 		DMA_FLAG_TEIF7
#define RS485_2_TX_DMA_FLAG_HTIF 		DMA_FLAG_HTIF7
#define RS485_2_TX_DMA_FLAG_TCIF 		DMA_FLAG_TCIF7

#define RS485_2_TX_DMA_IT_FEIF 			DMA_FLAG_FEIF7
#define RS485_2_TX_DMA_IT_DMEIF			DMA_IT_DMEIF7
#define RS485_2_TX_DMA_IT_TEIF 			DMA_IT_TEIF7
#define RS485_2_TX_DMA_IT_HTIF 			DMA_IT_HTIF7
#define RS485_2_TX_DMA_IT_TCIF 			DMA_IT_TCIF7

#define RS485_2_RX_DMA_CHANNEL			DMA_Channel_4
#define RS485_2_RX_DMA_STREAM			DMA2_Stream5
#define RS485_2_RX_DMA_FLAG_FEIF 		DMA_FLAG_FEIF5
#define RS485_2_RX_DMA_FLAG_DMEIF		DMA_FLAG_DMEIF5
#define RS485_2_RX_DMA_FLAG_TEIF 		DMA_FLAG_TEIF5
#define RS485_2_RX_DMA_FLAG_HTIF 		DMA_FLAG_HTIF5
#define RS485_2_RX_DMA_FLAG_TCIF 		DMA_FLAG_TCIF5

#define RS485_2_RX_DMA_IT_FEIF 			DMA_FLAG_FEIF5
#define RS485_2_RX_DMA_IT_DMEIF			DMA_IT_DMEIF5
#define RS485_2_RX_DMA_IT_TEIF 			DMA_IT_TEIF5
#define RS485_2_RX_DMA_IT_HTIF 			DMA_IT_HTIF5
#define RS485_2_RX_DMA_IT_TCIF 			DMA_IT_TCIF5

#define RS485_2_DMA_TX_IRQn				DMA2_Stream7_IRQn
#define RS485_2_DMA_RX_IRQn				DMA2_Stream5_IRQn
#define RS485_2_DMA_TX_IRQHandler		DMA2_Stream7_IRQHandler
#define RS485_2_DMA_RX_IRQHandler		DMA2_Stream5_IRQHandler

#define RS485BUFFERSIZE 50

extern rt_thread_t rs485_thread;

static void RS485_1_Config(void);
void RS485_1_Dir_Set(RS485_DIR dir);
void RS485_1_Subport(RS485_SUBPORT port);
static void RS485_2_Config(void);
void RS485_2_Dir_Set(RS485_DIR dir);
void RS485_2_Subport(RS485_SUBPORT port);
void RS485_thread_entry(void* parameter);

#endif
