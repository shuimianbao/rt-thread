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

#define RS485_1                        USART6
#define RS485_1_CLK                    RCC_APB2Periph_USART6
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
#define RS485_1_ISR										 USART6_IRQHandler
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

extern rt_thread_t rs485_thread;

static void RS485_1_Config(void);
void RS485_1_Dir_Set(RS485_DIR dir);
void RS485_1_Subport(RS485_SUBPORT port);
static void RS485_2_Config(void);
void RS485_2_Dir_Set(RS485_DIR dir);
void RS485_2_Subport(RS485_SUBPORT port);
void RS485_thread_entry(void* parameter);

#endif
