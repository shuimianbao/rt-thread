#include <rtthread.h>
#include "oled.h"

void OLED_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* OLED_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(OLED_I2C_CLK, ENABLE);
    
  /* Configure the I2C clock source. The clock is derived from the HSI */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
  
  /* OLED_I2C_SCL_GPIO_CLK, OLED_I2C_SDA_GPIO_CLK 
       and OLED_I2C_SMBUSALERT_GPIO_CLK Periph clock enable */
  RCC_AHBPeriphClockCmd(OLED_I2C_SCL_GPIO_CLK | OLED_I2C_SDA_GPIO_CLK, ENABLE);
  
  /* Connect PXx to I2C_SCL */
  GPIO_PinAFConfig(OLED_I2C_SCL_GPIO_PORT, OLED_I2C_SCL_SOURCE, OLED_I2C_SCL_AF);

  /* Connect PXx to I2C_SDA */
  GPIO_PinAFConfig(OLED_I2C_SDA_GPIO_PORT, OLED_I2C_SDA_SOURCE, OLED_I2C_SDA_AF); 

    
  /* Configure OLED_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = OLED_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(OLED_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /* Configure OLED_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = OLED_I2C_SDA_PIN;
  GPIO_Init(OLED_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

}
void OLED_Init(void)
{  
  I2C_InitTypeDef  I2C_InitStructure;
  
  OLED_LowLevel_Init();
  
  /* OLED_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = OLED_I2C_TIMING;
  
  /* Apply OLED_I2C configuration after enabling it */
  I2C_Init(OLED_I2C, &I2C_InitStructure);
  
  /* OLED_I2C Peripheral Enable */
  I2C_Cmd(OLED_I2C, ENABLE);
}