#include <rtthread.h>
#include "stm32f4xx.h"
#include "eeprom.h"

__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   
__IO uint16_t* sEEDataReadPointer;   
__IO uint8_t*  sEEDataWritePointer;  
__IO uint8_t   sEEDataNum;
rt_thread_t eeprom_thread;

//uint8_t writebuf[] = "ABCDEFG";

uint8_t aTxBuffer1[] = "/* STM32F40xx I2C Firmware Library EEPROM driver example: \
                        buffer 1 transfer into address sEE_WRITE_ADDRESS1 */ \
                        Example Description \
                        This firmware provides a basic example of how to use the I2C firmware library and\
                        an associate I2C EEPROM driver to communicate with an I2C EEPROM device (here the\
                        example is interfacing with M24C64 EEPROM)\
                          \
                        I2C peripheral is configured in Master transmitter during write operation and in\
                        Master receiver during read operation from I2C EEPROM. \
                          \
                        The peripheral used is I2C1 but can be configured by modifying the defines values\
                        in stm324xg_eval.h file. The speed is set to 100kHz and can be configured by \
                        modifying the relative define in stm32_eval_i2c_ee.h file.\
                         \
                        For M24C64 devices all the memory is accessible through the two-bytes \
                        addressing mode and need to define block addresses. In this case, only the physical \
                        address has to be defined (according to the address pins (E0,E1 and E2) connection).\
                        This address is defined in i2c_ee.h (default is 0xA0: E0, E1 and E2 tied to ground).\
                        The EEPROM addresses where the program start the write and the read operations \
                        is defined in the main.c file. \
                         \
                        First, the content of Tx1_Buffer is written to the EEPROM_WriteAddress1 and the\
                        written data are read. The written and the read buffers data are then compared.\
                        Following the read operation, the program waits that the EEPROM reverts to its \
                        Standby state. A second write operation is, then, performed and this time, Tx2_Buffer\
                        is written to EEPROM_WriteAddress2, which represents the address just after the last \
                        written one in the first write. After completion of the second write operation, the \
                        written data are read. The contents of the written and the read buffers are compared.\
                         \
                        All transfers are managed in DMA mode (except when 1-byte read/write operation is\
                        required). Once sEE_ReadBuffer() or sEE_WriteBuffer() function is called, the \
                        use application may perform other tasks in parallel while Read/Write operation is\
                        managed by DMA.\
                          \
                        This example provides the possibility to use the STM324xG-EVAL LCD screen for\
                        messages display (transfer status: Ongoing, PASSED, FAILED).\
                        To enable this option uncomment the define ENABLE_LCD_MSG_DISPLAY in the main.c\
                        file.                                                                              ";
uint8_t readbuf[sizeof(aTxBuffer1)];
static void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  //NVIC_InitTypeDef NVIC_InitStructure;
  //sEE_LowLevel_Init();
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*!< sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
  
  /*!< sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Reset sEE_I2C IP */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, ENABLE);
  
  /* Release reset signal of sEE_I2C IP */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, DISABLE);
    
  /*!< GPIO configuration */
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_SOURCE, sEE_I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_SOURCE, sEE_I2C_SDA_AF);  
  
  /*!< Configure sEE_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
     
  /*!< I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(sEE_I2C, &I2C_InitStructure);

  
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0) 
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#if 0
      /* Wait transfer through DMA to be complete */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
      }
#endif
      sEE_WaitEepromStandbyState();
			
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;        
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }     
#endif				
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }   
#endif				
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;        
        /*!< Write the data contained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        } 
#endif				
        sEE_WaitEepromStandbyState();      
        
        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }     
#endif
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        sEEDataNum = count;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }   
#endif				
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }        
#endif
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
#if 0
        /* Wait transfer through DMA to be complete */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }    
#endif				
        sEE_WaitEepromStandbyState();
      }
    }
  }  
}
/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  *         This function doesn't check on boundaries condition (in this driver 
  *         the function sEE_WriteBuffer() which calls sEE_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to 
  *         be written into the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the 
  *              data are written to the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note This function just configure the communication and enable the DMA 
  *       channel to transfer data. Meanwhile, the user application may perform 
  *       other tasks in parallel.
  * 
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
  /* Set the pointer to the Number of data to be written. This pointer will be used 
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  sEEDataWritePointer = NumByteToWrite;  
  
  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send EEPROM address for write */
  sEETimeout = sEE_FLAG_TIMEOUT;
  I2C_Send7bitAddress(sEE_I2C, sEE_HW_ADDRESS, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }


  
  /*!< Send the EEPROM's internal address to write to : MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /*!< Send the EEPROM's internal address to write to : LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
 
	for(uint8_t i=0; i<*NumByteToWrite; i++)
	{
		/*!< Send the EEPROM's internal address to write to : LSB of the address */
		I2C_SendData(sEE_I2C, pBuffer[i]);
		/*!< Test on EV8 and clear it */
		sEETimeout = sEE_FLAG_TIMEOUT; 
		while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
		{
			if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
		}  
	}
	/*!< Wait till all data have been physically transferred on the bus */
	sEETimeout = sEE_LONG_TIMEOUT;
	while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
	{
		if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback();
	}
    
	/*!< Send STOP condition */
	I2C_GenerateSTOP(sEE_I2C, ENABLE);
#if 0	
  /* Configure the DMA Tx Channel with the buffer address and the buffer size */
  sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
  
  /* Enable the DMA Tx Stream */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, ENABLE);
  
  /* Enable the sEE_I2C peripheral DMA requests */
  I2C_DMACmd(sEE_I2C, ENABLE);
#endif
  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}
/**
  * @brief  Wait for EEPROM Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  * 
  * @param  None
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_WaitEepromStandbyState(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /* Keep looping till the slave acknowledge his address or maximum number 
     of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
     in stm324xg_eval_i2c_ee.h file) */
  while (1)
  {
    /*!< Send START condition */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /*!< Test on EV5 and clear it */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }    

    /*!< Send EEPROM address for write */
    I2C_Send7bitAddress(sEE_I2C, sEE_HW_ADDRESS, I2C_Direction_Transmitter);
    
    /* Wait for ADDR flag to be set (Slave acknowledged his address) */
    sEETimeout = sEE_LONG_TIMEOUT;
    do
    {     
      /* Get the current value of the SR1 register */
      tmpSR1 = sEE_I2C->SR1;
      
      /* Update the timeout value and exit if it reach 0 */
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
    /* Keep looping till the Address is acknowledged or the AF flag is 
       set (address not acknowledged at time) */
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
    /* Check if the ADDR flag has been set */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
      /* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
         been read) */
      (void)sEE_I2C->SR2;
      
      /*!< STOP condition */    
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
        
      /* Exit the function */
      return sEE_OK;
    }
    else
    {
      /*!< Clear AF flag */
      I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
    }
    
    /* Check if the maximum allowed number of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return sEE_TIMEOUT_UserCallback();
    }
  }
}
/**
  * @brief  Basic management of the timeout situation.
  * @param  None.
  * @retval None.
  */
uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
	/*!< Send STOP condition */
	I2C_GenerateSTOP(sEE_I2C, ENABLE);
	rt_kprintf("EEEPROM Time out\n");
	return sEE_FAIL;
  //while (1)
 // {   
  //}
}
/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to start reading from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         be read from the EEPROM.
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be read is higher than 1, this function just 
  *       configures the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used. The byte
  *       is read in polling mode.
  * 
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
  */
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead)
{  
  /* Set the pointer to the Number of data to be read. This pointer will be used 
      by the DMA Transfer Completer interrupt Handler in order to reset the 
      variable to 0. User should check on this variable in order to know if the 
      DMA transfer has been complete or not. */
  //sEEDataReadPointer = NumByteToRead;
  
  /*!< While the bus is busy */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send START condition */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send EEPROM address for write */
  I2C_Send7bitAddress(sEE_I2C, sEE_HW_ADDRESS, I2C_Direction_Transmitter);

  /*!< Test on EV6 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 


  /*!< Send the EEPROM's internal address to read from: MSB of the address first */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*!< Send the EEPROM's internal address to read from: LSB of the address */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
  

  /*!< Test on EV8 and clear it */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*!< Send STRAT condition a second time */  
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*!< Test on EV5 and clear it (cleared by reading SR1 then writing to DR) */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 
  
  /*!< Send EEPROM address for read */
  I2C_Send7bitAddress(sEE_I2C, sEE_HW_ADDRESS, I2C_Direction_Receiver);  
	
	/* Wait on ADDR flag to be set (ADDR is still not cleared at this level */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }     
    
    /*!< Re-Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);     
    
    /* Clear ADDR register by reading SR1 then SR2 register (SR1 has already been read) */
    (void)sEE_I2C->SR2;
		
	for(uint16_t cnt=0; cnt < (uint16_t)(NumByteToRead)-1; cnt++)
	{
		/* Wait for the byte to be received */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
		*pBuffer = I2C_ReceiveData(sEE_I2C);
		pBuffer++;
	}
	
	/*!< Send STOP Condition */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    /* Wait for the byte to be received */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
    
    /*!< Read the byte received from the EEPROM */
    *pBuffer = I2C_ReceiveData(sEE_I2C);
      
    
    /* Wait to make sure that STOP control bit has been cleared */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(sEE_I2C->CR1 & I2C_CR1_STOP)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /*!< Re-Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE); 
  
  
  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

void eeprom_thread_entry(void* parameter)
{
	uint16_t i=0;
	sEE_Init();
	//rt_kprintf("extio_thread_entry running\n");
	sEE_WriteBuffer(aTxBuffer1,0x50, sizeof(aTxBuffer1));
	for(i=0;i<sizeof(aTxBuffer1);i++)
		readbuf[i]= 0;
	sEE_ReadBuffer(readbuf,0x50,sizeof(aTxBuffer1));
	
	rt_kprintf("read eeprom: \n");
	for(i=0;i<sizeof(aTxBuffer1);i++)
	{
		if(aTxBuffer1[i] != readbuf[i])
		{
			rt_kprintf("error %d,%c,%c: %s\n",i,aTxBuffer1[i],readbuf[i]);
			break;
		}
		else
			rt_kprintf("%c",readbuf[i]);
	}
	rt_kprintf("\nread end %d\n",sizeof(aTxBuffer1));
	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		;
		
	}
}	
