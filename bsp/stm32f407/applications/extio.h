#ifndef _EXTIO_H_
#define _EXTIO_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define EXT_OUT1_GPIO_CLK								RCC_AHB1Periph_GPIOD
#define EXT_OUT1_PIN										GPIO_Pin_12
#define EXT_OUT1_PORT										GPIOD
#define EXT_OUT2_GPIO_CLK								RCC_AHB1Periph_GPIOD
#define EXT_OUT2_PIN										GPIO_Pin_11
#define EXT_OUT2_PORT										GPIOD
#define EXT_OUT3_GPIO_CLK								RCC_AHB1Periph_GPIOD
#define EXT_OUT3_PIN										GPIO_Pin_10
#define EXT_OUT3_PORT										GPIOD
#define EXT_OUT4_GPIO_CLK								RCC_AHB1Periph_GPIOD
#define EXT_OUT4_PIN										GPIO_Pin_9
#define EXT_OUT4_PORT										GPIOD
#define EXT_OUT5_GPIO_CLK								RCC_AHB1Periph_GPIOD
#define EXT_OUT5_PIN										GPIO_Pin_8
#define EXT_OUT5_PORT										GPIOD
#define EXT_OUT6_GPIO_CLK								RCC_AHB1Periph_GPIOB
#define EXT_OUT6_PIN										GPIO_Pin_15
#define EXT_OUT6_PORT										GPIOB
#define EXT_OUT7_GPIO_CLK								RCC_AHB1Periph_GPIOB
#define EXT_OUT7_PIN										GPIO_Pin_14
#define EXT_OUT7_PORT										GPIOB
#define EXT_OUT8_GPIO_CLK								RCC_AHB1Periph_GPIOB
#define EXT_OUT8_PIN										GPIO_Pin_13
#define EXT_OUT8_PORT										GPIOB

extern rt_thread_t extio_thread;

static void Ext_Out_Config(void);
void Ext_Out_SetBits(uint16_t GPIO_Pin);
void Ext_Out_ResetBits(uint16_t GPIO_Pin);
void extio_thread_entry(void* parameter);
#endif
