#ifndef _GPS_H_
#define _GPS_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define GPS_COM                        UART5
#define GPS_COM_CLK                    RCC_APB1Periph_UART5
#define GPS_COM_TX_PIN                 GPIO_Pin_12
#define GPS_COM_TX_GPIO_PORT           GPIOC
#define GPS_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define GPS_COM_TX_SOURCE              GPIO_PinSource12
#define GPS_COM_TX_AF                  GPIO_AF_UART5
#define GPS_COM_RX_PIN                 GPIO_Pin_2
#define GPS_COM_RX_GPIO_PORT           GPIOD
#define GPS_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define GPS_COM_RX_SOURCE              GPIO_PinSource2
#define GPS_COM_RX_AF                  GPIO_AF_UART5
#define GPS_COM_IRQn                   UART5_IRQn
#define GPS_COM_ISR										 UART5_IRQHandler
#define GPS_COM_RST_GPIO_CLK           	 RCC_AHB1Periph_GPIOB
#define GPS_COM_RST_PIN								 GPIO_Pin_4
#define GPS_COM_RST_PORT							   GPIOB
#define GPS_COM_1PPS_GPIO_CLK           	 RCC_AHB1Periph_GPIOB
#define GPS_COM_1PSS_PIN								 GPIO_Pin_5
#define GPS_COM_1PPS_PORT							   GPIOB

extern rt_thread_t gps_thread;

static void GPS_COM_Config(void);
void gps_thread_entry(void* parameter);

#endif

