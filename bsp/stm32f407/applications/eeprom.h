#ifndef _EEPROM_H_
#define _EEPROM_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource4
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1
#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.07 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource4
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1
#define I2C_SPEED                        100000
#define I2C_SLAVE_ADDRESS7      				 0xA0
#define sEE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 
#define sEE_PAGESIZE           				 	 128
//#define sEE_WRITE_ADDRESS1        			 0x50
//#define sEE_READ_ADDRESS1         			 0x50
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(30 * sEE_FLAG_TIMEOUT))
/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300
/* Defintions for the state of the DMA transfer */   
#define sEE_STATE_READY           0
#define sEE_STATE_BUSY            1
#define sEE_STATE_ERROR           2
#define sEE_OK                    0
#define sEE_FAIL                  1  


extern rt_thread_t eeprom_thread;


static void sEE_Init(void);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);   
uint32_t sEE_TIMEOUT_UserCallback(void);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);
void eeprom_thread_entry(void* parameter);
#endif

