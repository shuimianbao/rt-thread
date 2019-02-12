#include "stm32f4xx.h"
#include "spi1.h"
static void SPI1_gpio_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable the SPI clock */
  LCD_SPI_CLK_INIT(LCD_SPI_CLK, ENABLE);

  /*!< Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(LCD_SPI_SCK_GPIO_CLK | LCD_SPI_MISO_GPIO_CLK | 
                         LCD_SPI_MOSI_GPIO_CLK | LCD_CS_GPIO_CLK, ENABLE);
  
  /*!< SPI pins configuration *************************************************/

  /*!< Connect SPI pins to AF5 */  
  GPIO_PinAFConfig(LCD_SPI_SCK_GPIO_PORT, LCD_SPI_SCK_SOURCE, LCD_SPI_SCK_AF);
  GPIO_PinAFConfig(LCD_SPI_MISO_GPIO_PORT, LCD_SPI_MISO_SOURCE, LCD_SPI_MISO_AF);
  GPIO_PinAFConfig(LCD_SPI_MOSI_GPIO_PORT, LCD_SPI_MOSI_SOURCE, LCD_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
        
  /*!< SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
  GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  LCD_SPI_MOSI_PIN;
  GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /*!< SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin =  LCD_SPI_MISO_PIN;
  GPIO_Init(LCD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure LCD Card CS pin in output pushpull mode ********************/
  GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(LCD_CS_GPIO_PORT, &GPIO_InitStructure);
	LCD_CS_HIGH();
//	GPIO_InitTypeDef  GPIO_InitStruct;	
//	__HAL_RCC_GPIOA_CLK_ENABLE();
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_5/*SCK*/ | GPIO_PIN_6/*MISO*/ | GPIO_PIN_7/*MOSI*/;
//	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//	GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_4/*NSS*/;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Pull = GPIO_PULLUP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
//	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//	GPIOA->BSRR = GPIO_PIN_4;
}

void SPI1_init(void)
{
//	SPI_HandleTypeDef SpiHandle;
	SPI_InitTypeDef  SPI_InitStructure;
	SPI1_gpio_init();

	/*!< SPI configuration */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;

	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

  SPI_Init(LCD_SPI, &SPI_InitStructure);

  /*!< Enable the sFLASH_SPI  */
  SPI_Cmd(LCD_SPI, ENABLE);
	
//	__HAL_RCC_SPI1_CLK_ENABLE();
//	
//	SpiHandle.Instance               = SPI1;
//	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;	// 84MHz/VAL
//	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
//	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
//	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
//	SpiHandle.Init.DataSize          = SPI_DATASIZE_16BIT;
//	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
//	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
//	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
//	SpiHandle.Init.CRCPolynomial     = 7;
//	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
//	SpiHandle.Init.Mode              = SPI_MODE_MASTER;
//	if(HAL_SPI_Init(&SpiHandle) != HAL_OK) while(1);
//	__HAL_SPI_ENABLE(&SpiHandle);
}



uint16_t SPI1_transfer(uint16_t send_d)
{
//	GPIOA->BSRR = GPIO_PIN_4<<16;
//	SPI1->DR = send_d;
//	while(!(SPI1->SR & SPI_FLAG_RXNE));
//	GPIOA->BSRR = GPIO_PIN_4;
//	return SPI1->DR;
	LCD_CS_LOW();
	/*!< Loop while DR register in not empty */
  while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /*!< Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(LCD_SPI, send_d);

  /*!< Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_RXNE) == RESET);
 
	LCD_CS_HIGH();
	 /*!< Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(LCD_SPI);
	
}




