#include <rtthread.h>
#include "stm32f4xx.h"
#include "rs485.h"


rt_uint8_t RS485_1_DMA_RxBuf[RS485BUFFERSIZE];
rt_uint8_t RS485_1_DMA_TxBuf[RS485BUFFERSIZE];
rt_uint8_t RS485_2_DMA_RxBuf[RS485BUFFERSIZE];
rt_uint8_t RS485_2_DMA_TxBuf[RS485BUFFERSIZE];

rt_thread_t rs485_thread;

void RS485_1_Dir_Set(RS485_DIR dir)
{
	if(dir == RS485_RECV)
		GPIO_ResetBits(RS485_1_DIR_PORT, RS485_1_DIR_PIN);
	else
		GPIO_SetBits(RS485_1_DIR_PORT, RS485_1_DIR_PIN);
}


void RS485_1_Subport(RS485_SUBPORT port)
{
	if(port == RS485_SUBPORT1)//00
	{
		GPIO_ResetBits(RS485_1_A1_PORT, RS485_1_A1_PIN);
		GPIO_ResetBits(RS485_1_A2_PORT, RS485_1_A2_PIN);
	}
	else if(port == RS485_SUBPORT2)//01
	{
		GPIO_SetBits(RS485_1_A1_PORT, RS485_1_A1_PIN);
		GPIO_ResetBits(RS485_1_A2_PORT, RS485_1_A2_PIN);
	}	
	else if(port == RS485_SUBPORT3)//10
	{
		GPIO_ResetBits(RS485_1_A1_PORT, RS485_1_A1_PIN);
		GPIO_SetBits(RS485_1_A2_PORT, RS485_1_A2_PIN);
	}	
	else//11
	{
		GPIO_SetBits(RS485_1_A1_PORT, RS485_1_A1_PIN);
		GPIO_SetBits(RS485_1_A2_PORT, RS485_1_A2_PIN);
	}	
}
static void RS485_1_Config(void)
{
 	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
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
  	RCC_AHB1PeriphClockCmd(RS485_1_TX_GPIO_CLK | RS485_1_RX_GPIO_CLK, ENABLE);

    /* Enable UART clock */
  	RCC_APB2PeriphClockCmd(RS485_1_CLK, ENABLE);


  	/* Connect PXx to USARTx_Tx*/
  	GPIO_PinAFConfig(RS485_1_TX_GPIO_PORT, RS485_1_TX_SOURCE, RS485_1_TX_AF);

  	/* Connect PXx to USARTx_Rx*/
  	GPIO_PinAFConfig(RS485_1_RX_GPIO_PORT, RS485_1_RX_SOURCE, RS485_1_RX_AF);

  	/* Configure USART Tx as alternate function  */
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  	GPIO_InitStructure.GPIO_Pin = RS485_1_TX_PIN;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  	GPIO_Init(RS485_1_TX_GPIO_PORT, &GPIO_InitStructure);

  	/* Configure USART Rx as alternate function  */
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_Pin = RS485_1_RX_PIN;
  	GPIO_Init(RS485_1_RX_GPIO_PORT, &GPIO_InitStructure);

  	/* USART configuration */
  	USART_Init(RS485_1, &USART_InitStructure);
  	/* Enable the Tx buffer empty interrupt */
  	//USART_ITConfig(RS485_1, USART_IT_TXE, ENABLE);
	/* Enable the Tx buffer empty interrupt */
  	//USART_ITConfig(RS485_1, USART_IT_RXNE, ENABLE); //RX by DMA	
  	USART_ITConfig(RS485_1, USART_IT_IDLE, ENABLE); //IDLE interrupt
  


	//RS485_1_DIR		PD15
  	//RS485_1_A1		PD13
	//RS485_1_A2		PD14
	/* GPIOG Peripheral clock enable */
  	RCC_AHB1PeriphClockCmd(RS485_1_DIR_GPIO_CLK | RS485_1_A1_GPIO_CLK | RS485_1_A2_GPIO_CLK, ENABLE);

  	/* Configure PG6 and PG8 in output pushpull mode */
  	GPIO_InitStructure.GPIO_Pin = RS485_1_DIR_PIN |RS485_1_A1_PIN | RS485_1_A2_PIN;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  	GPIO_Init(RS485_1_DIR_PORT, &GPIO_InitStructure);
	RS485_1_Dir_Set(RS485_RECV);

	/* Enable the DMA clock */
  	RCC_AHB1PeriphClockCmd(RS485_1_DMA_CLK , ENABLE);

  	/* Configure DMA Initialization Structure */
  	DMA_InitStructure.DMA_BufferSize = RS485BUFFERSIZE ;
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full ;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_PeripheralBaseAddr =(rt_uint32_t) (&(RS485_1->DR)) ;//notice
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  	
  	/* Configure RS485_1 TX DMA */
  	DMA_InitStructure.DMA_Channel = RS485_1_TX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  	DMA_InitStructure.DMA_Memory0BaseAddr =(rt_uint32_t)RS485_1_DMA_TxBuf ;
  	DMA_Init(RS485_1_TX_DMA_STREAM,&DMA_InitStructure);
	/* Configure RS485_1 RX DMA */
 	DMA_InitStructure.DMA_Channel =  RS485_1_RX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_Memory0BaseAddr =(rt_uint32_t)RS485_1_DMA_RxBuf ; 
  	DMA_Init(RS485_1_RX_DMA_STREAM,&DMA_InitStructure);
		
	/* Clear any pending flag on RS485_1 Tx Stream */
  	DMA_ClearFlag(RS485_1_TX_DMA_STREAM, RS485_1_TX_DMA_FLAG_FEIF | RS485_1_TX_DMA_FLAG_DMEIF | RS485_1_TX_DMA_FLAG_TEIF | \
                                       RS485_1_TX_DMA_FLAG_HTIF | RS485_1_TX_DMA_FLAG_TCIF);
  	/* Clear any pending flag on RS485_1 Rx Stream */
  	DMA_ClearFlag(RS485_1_RX_DMA_STREAM, RS485_1_RX_DMA_FLAG_FEIF | RS485_1_RX_DMA_FLAG_DMEIF | RS485_1_RX_DMA_FLAG_TEIF | \
                                       RS485_1_RX_DMA_FLAG_HTIF | RS485_1_RX_DMA_FLAG_TCIF);

  	/* Enable the DMA Channels Interrupts */
	DMA_ITConfig(RS485_1_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
  	DMA_ITConfig(RS485_1_RX_DMA_STREAM, DMA_IT_TC | DMA_IT_HT, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* NVIC configuration */
  	/* Enable the USARTx Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = RS485_1_IRQn;//IDLE interrput
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = RS485_1_DMA_TX_IRQn;
  	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = RS485_1_DMA_RX_IRQn;
  	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(RS485_1_TX_DMA_STREAM,ENABLE); 
	DMA_Cmd(RS485_1_RX_DMA_STREAM,ENABLE);  
  	/* Enable USART DMA RX Requsts */
  	USART_DMACmd(RS485_1, USART_DMAReq_Rx, ENABLE);
	/* Enable USART */
  	USART_Cmd(RS485_1, ENABLE);
}

void RS485_2_Dir_Set(RS485_DIR dir)
{
	if(dir == RS485_RECV)
		GPIO_ResetBits(RS485_2_DIR_PORT, RS485_2_DIR_PIN);
	else
		GPIO_SetBits(RS485_2_DIR_PORT, RS485_2_DIR_PIN);
}

void RS485_2_Subport(RS485_SUBPORT port)
{
	if(port == RS485_SUBPORT1)//00
	{
		GPIO_ResetBits(RS485_2_A1_PORT, RS485_2_A1_PIN);
		GPIO_ResetBits(RS485_2_A2_PORT, RS485_2_A2_PIN);
	}
	else if(port == RS485_SUBPORT2)//01
	{
		GPIO_SetBits(RS485_2_A1_PORT, RS485_2_A1_PIN);
		GPIO_ResetBits(RS485_2_A2_PORT, RS485_2_A2_PIN);
	}	
	else if(port == RS485_SUBPORT3)//10
	{
		GPIO_ResetBits(RS485_2_A1_PORT, RS485_2_A1_PIN);
		GPIO_SetBits(RS485_2_A2_PORT, RS485_2_A2_PIN);
	}	
	else//11
	{
		GPIO_SetBits(RS485_2_A1_PORT, RS485_2_A1_PIN);
		GPIO_SetBits(RS485_2_A2_PORT, RS485_2_A2_PIN);
	}	
}

static void RS485_2_Config(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;
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
  RCC_AHB1PeriphClockCmd(RS485_2_TX_GPIO_CLK | RS485_2_RX_GPIO_CLK, ENABLE);

    /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RS485_2_CLK, ENABLE);


  /* Connect PXx to USARTx_Tx*/
  GPIO_PinAFConfig(RS485_2_TX_GPIO_PORT, RS485_2_TX_SOURCE, RS485_2_TX_AF);

  /* Connect PXx to USARTx_Rx*/
  GPIO_PinAFConfig(RS485_2_RX_GPIO_PORT, RS485_2_RX_SOURCE, RS485_2_RX_AF);

  /* Configure USART Tx as alternate function  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = RS485_2_TX_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_2_TX_GPIO_PORT, &GPIO_InitStructure);

  /* Configure USART Rx as alternate function  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = RS485_2_RX_PIN;
  GPIO_Init(RS485_2_RX_GPIO_PORT, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(RS485_2, &USART_InitStructure);
  /* Enable the Tx buffer empty interrupt */
  //USART_ITConfig(RS485_2, USART_IT_TXE, ENABLE);
	/* Enable the Tx buffer empty interrupt */
  //USART_ITConfig(RS485_2, USART_IT_RXNE, ENABLE); //using DMA for RX
  USART_ITConfig(RS485_2, USART_IT_IDLE, ENABLE); //IDLE interrupt
  
	//RS485_2_DIR		PC8
  //RS485_2_A1		PC9
	//RS485_2_A2		PA8
/* GPIOG Peripheral clock enable */
  RCC_AHB1PeriphClockCmd(RS485_2_DIR_GPIO_CLK | RS485_2_A1_GPIO_CLK | RS485_2_A2_GPIO_CLK, ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = RS485_2_DIR_PIN |RS485_2_A1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RS485_2_DIR_PORT, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RS485_2_DIR_GPIO_CLK | RS485_2_A1_GPIO_CLK , ENABLE);

  /* Configure PG6 and PG8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = RS485_2_A2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(RS485_2_A2_PORT, &GPIO_InitStructure);
	
	RS485_2_Dir_Set(RS485_RECV);
	 
	/* Enable the DMA clock */
  	RCC_AHB1PeriphClockCmd( RS485_2_DMA_CLK, ENABLE);

  	/* Configure DMA Initialization Structure */
  	DMA_InitStructure.DMA_BufferSize = RS485BUFFERSIZE ;
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full ;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_PeripheralBaseAddr =(rt_uint32_t) (&(RS485_2->DR)) ;//notice
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	
  	/* Configure RS485_2 TX DMA */
  	DMA_InitStructure.DMA_Channel = RS485_2_TX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  	DMA_InitStructure.DMA_Memory0BaseAddr =(rt_uint32_t)RS485_2_DMA_TxBuf ;
  	DMA_Init(RS485_2_TX_DMA_STREAM,&DMA_InitStructure);
	/* Configure RS485_1 RX DMA */
 	DMA_InitStructure.DMA_Channel =  RS485_2_RX_DMA_CHANNEL ;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_Memory0BaseAddr =(rt_uint32_t)RS485_2_DMA_RxBuf ; 
  	DMA_Init(RS485_2_RX_DMA_STREAM,&DMA_InitStructure);
	
	/* Clear any pending flag on RS485_2 Tx Stream */
  	DMA_ClearFlag(RS485_2_TX_DMA_STREAM, RS485_2_TX_DMA_FLAG_FEIF | RS485_2_TX_DMA_FLAG_DMEIF | RS485_2_TX_DMA_FLAG_TEIF | \
                                       RS485_2_TX_DMA_FLAG_HTIF | RS485_2_TX_DMA_FLAG_TCIF);
  	/* Clear any pending flag on RS485_2 Rx Stream */
  	DMA_ClearFlag(RS485_2_RX_DMA_STREAM, RS485_2_RX_DMA_FLAG_FEIF | RS485_2_RX_DMA_FLAG_DMEIF | RS485_2_RX_DMA_FLAG_TEIF | \
                                       RS485_2_RX_DMA_FLAG_HTIF | RS485_2_RX_DMA_FLAG_TCIF);

  	/* Enable the DMA Channels Interrupts */
	DMA_ITConfig(RS485_2_TX_DMA_STREAM, DMA_IT_TC, ENABLE);
  	DMA_ITConfig(RS485_2_RX_DMA_STREAM, DMA_IT_TC | DMA_IT_HT, ENABLE);	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* NVIC configuration */
  	/* Enable the USARTx Interrupt */
  	NVIC_InitStructure.NVIC_IRQChannel = RS485_2_IRQn;//IDLE Interrput
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = RS485_2_DMA_TX_IRQn;
  	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = RS485_2_DMA_RX_IRQn;
  	NVIC_Init(&NVIC_InitStructure);

	DMA_Cmd(RS485_2_TX_DMA_STREAM,ENABLE); 
	DMA_Cmd(RS485_2_RX_DMA_STREAM,ENABLE);  
  	/* Enable USART DMA RX Requsts */
  	USART_DMACmd(RS485_2, USART_DMAReq_Rx, ENABLE);
	
	/* Enable USART */
  	USART_Cmd(RS485_2, ENABLE);
}

void RS485_thread_entry(void* parameter)
{
	RS485_1_Config();
	RS485_2_Config();

	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		//ShowADCResult();
	}
}

void RS485_1_ISR(void)
{
#if 0
	unsigned char ch;
	/* USART in Receiver mode */
	if (USART_GetITStatus(RS485_1, USART_IT_RXNE) == SET)
	{  
		ch = USART_ReceiveData(RS485_1);
		
		RS485_1_Dir_Set(RS485_SEND);
		USART_SendData(RS485_1, ch+1);
		while (USART_GetFlagStatus(RS485_1, USART_FLAG_TC) == RESET);
		RS485_1_Dir_Set(RS485_RECV); 
  }
	/* USART in Transmitter mode */
	if (USART_GetITStatus(RS485_1, USART_IT_TXE) == SET)
	{
	    
		/* Send Transaction data */
		//USART_SendData(USART6, aTxBuffer[ubTxIndex++]);
	   
	}
#endif

}

void RS485_2_ISR(void)
{
#if 0
	unsigned char ch;
  /* USART in Receiver mode */
  if (USART_GetITStatus(RS485_2, USART_IT_RXNE) == SET)
  {  
      ch = USART_ReceiveData(RS485_2);
		
			RS485_2_Dir_Set(RS485_SEND);
			USART_SendData(RS485_2, ch+1);
			while (USART_GetFlagStatus(RS485_2, USART_FLAG_TC) == RESET);
			RS485_2_Dir_Set(RS485_RECV);
     
  }
  /* USART in Transmitter mode */
  if (USART_GetITStatus(RS485_2, USART_IT_TXE) == SET)
  {
    
      /* Send Transaction data */
      //USART_SendData(RS485_2, aTxBuffer[ubTxIndex++]);
   
  }
 #endif
}
void RS485_1_DMA_TX_IRQHandler(void)
{

}
void RS485_1_DMA_RX_IRQHandler(void)
{

}

void RS485_2_DMA_TX_IRQHandler(void)
{

}
void RS485_2_DMA_RX_IRQHandler(void)
{

}


