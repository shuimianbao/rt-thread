#ifndef _LTE_H_
#define _LTE_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define LTE_COM                        UART4
#define LTE_COM_CLK                    RCC_APB1Periph_UART4
#define LTE_COM_TX_PIN                 GPIO_Pin_10
#define LTE_COM_TX_GPIO_PORT           GPIOC
#define LTE_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define LTE_COM_TX_SOURCE              GPIO_PinSource10
#define LTE_COM_TX_AF                  GPIO_AF_UART4
#define LTE_COM_RX_PIN                 GPIO_Pin_11
#define LTE_COM_RX_GPIO_PORT           GPIOC
#define LTE_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define LTE_COM_RX_SOURCE              GPIO_PinSource11
#define LTE_COM_RX_AF                  GPIO_AF_UART4
#define LTE_COM_IRQn                   UART4_IRQn
#define LTE_COM_ISR										 UART4_IRQHandler
#define LTE_COM_RST_GPIO_CLK           	 RCC_AHB1Periph_GPIOD
#define LTE_COM_RST_PIN								 GPIO_Pin_1
#define LTE_COM_RST_PORT							  GPIOD
#define LTE_COM_RELOAD_GPIO_CLK           	 RCC_AHB1Periph_GPIOD
#define LTE_COM_RELOAD_PIN								 GPIO_Pin_3
#define LTE_COM_RELOAD_PORT							   GPIOD
#define LTE_COM_WORK_GPIO_CLK           	 RCC_AHB1Periph_GPIOD
#define LTE_COM_WORK_PIN								 GPIO_Pin_4
#define LTE_COM_WORK_PORT							   GPIOD
#define LTE_COM_LINKB_GPIO_CLK           	 RCC_AHB1Periph_GPIOD
#define LTE_COM_LINKB_PIN								 GPIO_Pin_0
#define LTE_COM_LINKB_PORT							   GPIOD
#define LTE_COM_LINKA_GPIO_CLK           	 RCC_AHB1Periph_GPIOA
#define LTE_COM_LINKA_PIN								 GPIO_Pin_15
#define LTE_COM_LINKA_PORT							   GPIOA
 
extern rt_thread_t lte_thread;

static void LTE_COM_Config(void);
void lte_thread_entry(void* parameter);

#endif
