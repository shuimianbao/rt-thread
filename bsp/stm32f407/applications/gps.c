#include <rtthread.h>
#include "stm32f4xx.h"
#include "string.h"
#include "gps.h"

#include "lte.h"

rt_thread_t gps_thread;
static rt_int8_t GPS_DMA_RecBuf[GPSBUFFERSIZE];
static rt_uint8_t GPS_DMA_RecBuf_NewP=0;
static rt_uint8_t GPS_DMA_RecBuf_OldP=0;
static rt_uint8_t GPS_Buf_Flag=0;
static  struct  rt_semaphore  gps_sem;
static rt_int8_t GNMRC_Buf[80]={0};
static rt_uint8_t GNMRC_Buf_Index = 0;
static void GPS_COM_Config(void)
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
  //USART_ITConfig(GPS_COM, USART_IT_RXNE, ENABLE); //use DMA receive GPS data 
  
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


  /* Enable the DMA clock */
  RCC_AHB1PeriphClockCmd(GPS_DMA_CLK, ENABLE);

  /* Configure DMA Initialization Structure */
  DMA_InitStructure.DMA_BufferSize = GPSBUFFERSIZE ;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable ;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full ;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_PeripheralBaseAddr =(rt_uint32_t) (&(GPS_COM->DR)) ;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
  /* Configure TX DMA */
  //DMA_InitStructure.DMA_Channel = USARTx_TX_DMA_CHANNEL ;
  //DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ;
  //DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)aTxBuffer ;
  //DMA_Init(USARTx_TX_DMA_STREAM,&DMA_InitStructure);
  /* Configure RX DMA */
  DMA_InitStructure.DMA_Channel = GPS_RX_DMA_CHANNEL ;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ;
  DMA_InitStructure.DMA_Memory0BaseAddr =(rt_uint32_t)GPS_DMA_RecBuf ; 
  DMA_Init(GPS_RX_DMA_STREAM,&DMA_InitStructure);

  /* Clear any pending flag on Rx Stream */
  DMA_ClearFlag(GPS_RX_DMA_STREAM, GPS_RX_DMA_FLAG_FEIF | GPS_RX_DMA_FLAG_DMEIF | GPS_RX_DMA_FLAG_TEIF | \
                                       GPS_RX_DMA_FLAG_HTIF | GPS_RX_DMA_FLAG_TCIF);

  /* Enable the DMA Channels Interrupts */
  DMA_ITConfig(GPS_RX_DMA_STREAM, DMA_IT_TC | DMA_IT_HT, ENABLE);

  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  /* Enable the USARTx Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = GPS_COM_IRQn;
  NVIC_InitStructure.NVIC_IRQChannel = GPS_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	

  DMA_Cmd(GPS_RX_DMA_STREAM,ENABLE);  
  /* Enable USART DMA RX Requsts */
  USART_DMACmd(GPS_COM, USART_DMAReq_Rx, ENABLE);
  /* Enable USART */
  USART_Cmd(GPS_COM, ENABLE);
}

rt_uint8_t processdata(rt_uint8_t bufindex)
{
	rt_uint16_t start = bufindex;
	rt_uint16_t end = bufindex + GPSBUFFERSIZE/2;
	rt_uint16_t cp_st, cp_end,temp=0;
	rt_uint16_t i;
	
	while(start< end)
	{
		if(GPS_DMA_RecBuf[start] == '$')//search the first char '$'
		{
			if(((end-start)>= RMC_MAX_LEN) && ((strcmp(GPS_DMA_RecBuf[start+1], (const char *)("$GNRMC")) == 0)))//find the frame
			{
					cp_st = start;		
					break;
			}
		}//if(GPS_DMA_RecBuf[start] == '$')
		start++;
	}//while(start< end)
	if(start == end)//faild
			return 0;
	while(start< end)
	{
		if(GPS_DMA_RecBuf[start] == '\r')
		{
			cp_end = start;
			break;
		}
		start++;
	}
	
	if(start == end)//faild
			return 0;
	memcpy(GNMRC_Buf,GPS_DMA_RecBuf+cp_st,cp_end-cp_st+1);

	rt_kprintf("index=%d\n",bufindex);//test
	for(i=0;i<cp_end-cp_st+1;i++)
		rt_kprintf("%c",GNMRC_Buf[i]);

	//prase GNMRC
	
	
	
}
void gps_thread_entry(void* parameter)
{
	rt_err_t  result;
	rt_uint8_t i;
rt_uint8_t MRC_Serach_flag = MRC_SEARCH_NEW;
	GPS_COM_Config();
	result  =  rt_sem_init(&gps_sem,  "gpssem",  0,  RT_IPC_FLAG_FIFO);
	if  (result  !=  RT_EOK)
	{
		rt_kprintf("GPS sem init faild[%d]\n",result);
	}
	while(1)
	{
		//rt_thread_delay(RT_TICK_PER_SECOND*10);
		//ShowADCResult();
		 rt_sem_take(&gps_sem,  RT_WAITING_FOREVER);
		 if(GPS_Buf_Flag & 0xf)//HT
		 {
		 	//for(i=0;i<100;i++)
			//	rt_kprintf("%c",GPS_DMA_RecBuf[i]);
			// rt_kprintf("recv half len\n");
			// i = 
			GPS_Buf_Flag &=0xf0;
		 }
		  if(GPS_Buf_Flag & 0xf0)//TC
		 {
		 	//for(i=100;i<200;i++)
			//	rt_kprintf("%c",GPS_DMA_RecBuf[i]);
			//rt_kprintf("recv all len\n");

			GPS_Buf_Flag &=0x0f;
		 }
		 rt_sem_release(&gps_sem);
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

void GPS_DMA_RX_IRQHandler(void)
{
	if(DMA_GetITStatus(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_HTIF)==SET)
	{
		DMA_ClearITPendingBit(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_HTIF);
		GPS_Buf_Flag |= 0x1;
		rt_sem_release(&gps_sem);
	}

	if(DMA_GetITStatus(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_TCIF)==SET)
	{
		DMA_ClearITPendingBit(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_TCIF);
		GPS_Buf_Flag |= 0x10;
		rt_sem_release(&gps_sem);
	}
} 
