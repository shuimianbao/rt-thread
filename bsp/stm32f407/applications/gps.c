#include <rtthread.h>
#include "stm32f4xx.h"
#include "gps.h"

#include "lte.h"

rt_thread_t gps_thread;
static void GPS_COM_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  /* USARTx configured as follows:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(GPS_COM_TX_GPIO_CLK | GPS_COM_RX_GPIO_CLK, ENABLE);

    /* Enable UART clock */
  RCC_APB1PeriphClockCmd(GPS_COM_CLK, ENABLE);


  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(GPS_COM_TX_GPIO_PORT, GPS_COM_TX_SOURCE, GPS_COM_TX_AF);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(GPS_COM_RX_GPIO_PORT, GPS_COM_RX_SOURCE, GPS_COM_RX_AF);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = GPS_COM_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_Init(GPS_COM_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  //GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPS_COM_RX_PIN;
  GPIO_Init(GPS_COM_RX_GPIO_PORT, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(GPS_COM, &USART_InitStructure);
  /* Enable the Tx buffer empty interrupt */
  //USART_ITConfig(RS485_1, USART_IT_TXE, ENABLE);
	/* Enable the Tx buffer empty interrupt */
  USART_ITConfig(GPS_COM, USART_IT_RXNE, ENABLE);  
  
/* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(GPS_COM_RST_GPIO_CLK | GPS_COM_1PPS_GPIO_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPS_COM_RST_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPS_COM_RST_PORT, &GPIO_InitStructure);
	GPIO_SetBits(GPS_COM_RST_PORT, GPS_COM_RST_PIN);
	
	GPIO_InitStructure.GPIO_Pin = GPS_COM_1PSS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPS_COM_1PPS_PORT, &GPIO_InitStructure);	 
	
	/* NVIC configuration */
  /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = GPS_COM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART */
  USART_Cmd(GPS_COM, ENABLE);
}


void gps_thread_entry(void* parameter)
{
	GPS_COM_Config();

	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		//ShowADCResult();
	}
}

void GPS_COM_ISR(void)
{
unsigned char ch;
  /* USART in Receiver mode */
  if (USART_GetITStatus(GPS_COM, USART_IT_RXNE) == SET)
  {  
      ch = USART_ReceiveData(GPS_COM);
			//rt_kprintf("%c",ch);
		  //USART_SendData(LTE_COM, ch);
			//while (USART_GetFlagStatus(LTE_COM, USART_FLAG_TC) == RESET);
     
  }
  /* USART in Transmitter mode */
  if (USART_GetITStatus(GPS_COM, USART_IT_TXE) == SET)
  {
    
      /* Send Transaction data */
      //USART_SendData(RS485_2, aTxBuffer[ubTxIndex++]);
   
  }
}
