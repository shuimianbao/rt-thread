#ifndef _SPI1_H_
#define _SPI1_H_
#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"


#define LCD_SPI                           SPI1
#define LCD_SPI_CLK                       RCC_APB2Periph_SPI1
#define LCD_SPI_CLK_INIT                  RCC_APB2PeriphClockCmd

#define LCD_SPI_SCK_PIN                   GPIO_Pin_5
#define LCD_SPI_SCK_GPIO_PORT             GPIOA
#define LCD_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOA
#define LCD_SPI_SCK_SOURCE                GPIO_PinSource5
#define LCD_SPI_SCK_AF                    GPIO_AF_SPI1

#define LCD_SPI_MISO_PIN                  GPIO_Pin_6
#define LCD_SPI_MISO_GPIO_PORT            GPIOA
#define LCD_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define LCD_SPI_MISO_SOURCE               GPIO_PinSource6
#define LCD_SPI_MISO_AF                   GPIO_AF_SPI1

#define LCD_SPI_MOSI_PIN                  GPIO_Pin_7
#define LCD_SPI_MOSI_GPIO_PORT            GPIOA
#define LCD_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOA
#define LCD_SPI_MOSI_SOURCE               GPIO_PinSource7
#define LCD_SPI_MOSI_AF                   GPIO_AF_SPI1

#define LCD_CS_PIN                        GPIO_Pin_4
#define LCD_CS_GPIO_PORT                  GPIOA
#define LCD_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOA

/* Select LCD Chip Select pin low */
#define LCD_CS_LOW()       GPIO_ResetBits(LCD_CS_GPIO_PORT, LCD_CS_PIN)
/* Deselect LCD: Chip Select pin high */
#define LCD_CS_HIGH()      GPIO_SetBits(LCD_CS_GPIO_PORT, LCD_CS_PIN)  

void SPI1_init(void);
uint16_t SPI1_transfer(uint16_t send_d);


#endif








