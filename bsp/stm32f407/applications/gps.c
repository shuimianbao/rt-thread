#include <rtthread.h>
#include "stm32f4xx.h"
#include<stdio.h>
#include<string.h>
#include<math.h>

#include "gps.h"
//#include "gprmc.h"
//#include "lte.h"

rt_thread_t gps_thread;
char GPS_DMA_RecBuf[GPSBUFFERSIZE];
rt_uint8_t GPS_DMA_RecBuf_NewP=0;
rt_uint8_t GPS_DMA_RecBuf_OldP=0;
rt_uint8_t GPS_Buf_Flag=0;
struct  rt_semaphore  gps_sem;
char GNMRC_Buf[80]={0};
rt_uint8_t GNMRC_Buf_Index = 0;
GPS_INFO GPS_Data;

unsigned char CalcCheck(char* Bytes,int len)
{
	int i;
	unsigned char result;
	for (result = Bytes[0], i = 1; i < len ; i++)
	{
		result ^= Bytes[i];
	}
	return result;
}
//// 计算时区函数，根据经度来计算
char calculateTimezone(double lat,double lon)
{
 int a,b,c,timezone;
 a = (int)(fabs(lon)+0.5);// 对经度进行四舍五入，且取正整数
 b = a/15; // 商
 c = a%15; // 余数
 if((lat >=17.9 && lat <=53 && lon>=75 && lon<=125) || (lat>=40 && lat <=53 && lon>=125 && lon<= 135))
 {// 如果经纬度处于中国版图内，则都划为东8区，为什么要这样划分详见第三节 
	timezone = 8; 
 } 
 else
 {
	if(c > 7.5)
		timezone = b+1; 
	else 
		timezone = b; 
	
	if(lon > 0.0f)
		timezone = timezone;
	else 
		timezone = (-1)*timezone; 
 } 
 return (char)timezone; 
}

// UTC时间转换为本地时间函数 
void UTCTOLocalTime(char timezone,UTC_TIME utc_time,LOCAL_TIME * local_time)
{
 int year,month,day,hour;
 int lastday = 0;// 月的最后一天的日期
 int lastlastday = 0;// 上月的最后一天的日期
 
 year = utc_time.year; //已知的UTC时间
 month = utc_time.month;//已知的UTC时间
 day = utc_time.day;//已知的UTC时间
 hour = utc_time.hour + timezone; //已知的UTC时间
 
 if(month==1 || month==3 || month==5 || month==7 || month==8 || month==10 || month==12)
 { 
	lastday = 31; 
	if(month == 3)
	{
		if((year%400 == 0)||(year%4 == 0 && year%100 != 0))//判断是否为闰年，年号能被400整除或年号能被4整除，而不能被100整除为闰年
			lastlastday = 29;// 闰年的2月为29天，平年为28天
		else 
			lastlastday = 28; 
	} 
	
	if(month == 8) 
		lastlastday = 31; 
  } 
  else if(month == 4 || month == 6 || month == 9 || month == 11)
  { 
	lastday = 30; 
	lastlastday = 31; 
  } 
  else
  { 
	lastlastday = 31; 
	if((year%400 == 0)||(year%4 == 0 && year%100 != 0))// 闰年的2月为29天，平年为28天
		lastday = 29; 
	else 
		lastday = 28; 
   } 
   
   if(hour >= 24)
   {// 当算出的区时大于或等于24:00时，应减去24:00，日期加一天
		hour -= 24; 
		day += 1; 
		if(day > lastday)
		{ // 当算出的日期大于该月最后一天时，应减去该月最后一天的日期，月份加上一个月
			day -= lastday; 
			month += 1; 
			if(month > 12)
			{// 当算出的月份大于12时，应减去12，年份加上一年 
				month -= 12; 
				year += 1; 
			} 
		} 
	} 
	
	if(hour < 0)
	{// 当算出的区时为负数时，应加上24:00，日期减一天 
		hour += 24; 
		day -= 1; 
		if(day < 1)
		{ // 当算出的日期为0时，日期变为上一月的最后一天，月份减去一个月 
			day = lastlastday; 
			month -= 1; 
			if(month < 1)
			{ // 当算出的月份为0时，月份变为12月，年份减去一年 
				month = 12; 
				year -= 1; 
			} 
		} 
	} 
	//得到转换后的本地时间 
	local_time->year = year; 
	local_time->month = month; 
	local_time->day = day; 
	local_time->hour = hour; 
	local_time->minute = utc_time.minute;
	local_time->second = utc_time.second;
}


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
  DMA_ITConfig(GPS_RX_DMA_STREAM, DMA_IT_TC /*| DMA_IT_HT*/, ENABLE);//disable half transfer interrupt

  
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

/*
index=1
$GNRMC,164756.301,A,3202.8759,N,11855.7195,E,0.267,0.00,100419,,,A*49
index=0
$GNRMC,164759.000,A,3202.8759,N,11855.7182,E,0.132,0.00,100419,,,A*41
index=1
$GNRMC,164800.000,A,3202.8764,N,11855.7180,E,0.330,0.00,100419,,,A*4E
index=0
$GNRMC,164801.000,A,3202.8768,N,11855.7186,E,0.257,0.00,100419,,,A*45
index=1
$GNRMC,164802.000,A,3202.8776,N,11855.7196,E,0.235,0.00,100419,,,A*4C
index=0
$GNRMC,164804.000,A,3202.8787,N,11855.7216,E,0.266,0.00,100419,,,A*49
*/
#if 1
rt_uint8_t processdata(/*rt_uint8_t bufindex*/void)
{
	rt_uint16_t start = GPSBUFFERSIZE;
	rt_uint16_t end = GPSBUFFERSIZE;
	rt_uint16_t cp_st, cp_end,temp=0;
	rt_uint16_t i;
	
	unsigned char ret,lat_suffex, lon_suffex,position_valid,timezone;
	unsigned char len,x,checksum;
	unsigned int time,date;
	double latitude  ,longitude,du,fen; 
	
	do
	{
		start--;
		if(GPS_DMA_RecBuf[start] == '$')//search the first char '$'
		{
			if(((end-start)>= RMC_MAX_LEN) && ((memcmp(GPS_DMA_RecBuf+start+1, (const char *)("GNRMC"),5) == 0)))//find the frame
			{
					cp_st = start;		
					break;
			}
		}//if(GPS_DMA_RecBuf[start] == '$')
		
	}while(start>0);//reach the top of the buffer
		//while(start< end)
	if(start == 0)//faild
			return 0;
	while(start< end)
	{
		if(GPS_DMA_RecBuf[start] == '\n')
		{
			cp_end = start;
			break;
		}
		start++;
	}
	
	if(start == end)//faild
			return 0;
	memcpy(GNMRC_Buf,GPS_DMA_RecBuf+cp_st,cp_end-cp_st+1);

	//rt_kprintf("index=%d\n",bufindex);//test
	//for(i=0;i<cp_end-cp_st+1;i++)
	//	rt_kprintf("%c",GNMRC_Buf[i]);

	//prase GNMRC

	len = cp_end-cp_st+1;
	ret = CalcCheck(GNMRC_Buf+1,len-6);
	//rt_kprintf("check sum:%d,%x\n",ret,ret);

	for(x=0;x<len;x++)
	{
		if(GNMRC_Buf[x]=='*')
			break;		
	}
	if(GNMRC_Buf[x+1]<='9')
		checksum = (GNMRC_Buf[x+1]-'0')*16;
	else
		checksum = (GNMRC_Buf[x+1]-'A'+10)*16;
	
	if(GNMRC_Buf[x+2]<='9')
		checksum += (GNMRC_Buf[x+2]-'0');
	else
		checksum += (GNMRC_Buf[x+2]-'A'+10);
	
	//checksum = (GNMRC_Buf[x+1]-'0')*16+ (GNMRC_Buf[x+2]-'0');
	//printf("check sum1:%d,%x\n",checksum,checksum);
	if(ret != checksum)
	{
		rt_kprintf("check sum error[%d:%d]\n",ret,checksum);
		return 0;
	}
	
	sscanf(GNMRC_Buf,"$GNRMC,%d.%*d,%c,%lf,%c,%lf,%c,%*f,%*f,%d,%*s\r\n",
				           &time, &GPS_Data.vaild,
						   &latitude, &lat_suffex,
						   &longitude,&lon_suffex, 
						   &date);
	//rt_kprintf("lat:%f,lon:%f\n",latitude,longitude);					   
	GPS_Data.utc_time.second = 	time%100;
	time /=100;
	GPS_Data.utc_time.minute = time%100;
	time /=100;
	GPS_Data.utc_time.hour = time;
	
	GPS_Data.utc_time.year = date%100 +2000;
	date /=100;
	GPS_Data.utc_time.month = date%100;
	date /=100;
	GPS_Data.utc_time.day = date;
	
	//rt_kprintf("\n%lf\n",latitude);
	du = (double)(((long)latitude)/100);
	fen = latitude - du*100;
	//rt_kprintf("%lf,%lf\n",du,fen);
	GPS_Data.latitude = du+ fen/60.0;
	
	//rt_kprintf("\n%lf\n",longitude);
	du = (double)(((long)longitude)/100);
	fen = longitude - du*100;
	//rt_kprintf("%lf,%lf\n",du,fen);
	GPS_Data.longitude = du+ fen/60.0;
	
	if(lat_suffex == 'S')
		GPS_Data.latitude *=-1.0;
	if(lon_suffex == 'W')
		GPS_Data.longitude *=-1.0;
	
	//printf("time=%d,position_valid=%c,latitude=%lf,lat_suffex=%c,longitude=%lf,lon_suffex=%c,date=%d\n",
	//					   time, GPS_Data.vaild,
	//					   latitude, lat_suffex,
	//					   longitude,lon_suffex, 
	//					   date);
	timezone = calculateTimezone(GPS_Data.latitude,GPS_Data.longitude);
	UTCTOLocalTime(timezone,GPS_Data.utc_time,&(GPS_Data.local_time));
	//rt_kprintf("UTC TIME:%d-%d-%d %d:%d:%d\n",
	//		GPS_Data.utc_time.year,GPS_Data.utc_time.month,GPS_Data.utc_time.day,
	//		GPS_Data.utc_time.hour,GPS_Data.utc_time.minute,GPS_Data.utc_time.second);
	//rt_kprintf("LOCAL TIME:%d-%d-%d %d:%d:%d\n",
	//		GPS_Data.local_time.year,GPS_Data.local_time.month,GPS_Data.local_time.day,
	//		GPS_Data.local_time.hour,GPS_Data.local_time.minute,GPS_Data.local_time.second);
	//rt_kprintf("Position :%c,lat:%lf,lon:%lf\n",GPS_Data.vaild,GPS_Data.latitude,GPS_Data.longitude);

	return 1;
	
}
#else
rt_uint8_t processdata(rt_uint8_t bufindex)
{
	rt_uint16_t start = bufindex * GPSBUFFERSIZE/2;
	rt_uint16_t end = start + GPSBUFFERSIZE/2;
	rt_uint16_t cp_st, cp_end,temp=0;
	rt_uint16_t i;
	//
	/*
	nmeaINFO info;
    nmeaPARSER parser;
    nmea_zero_INFO(&info);
    nmea_parser_init(&parser);
	*/
	while(start< end)
	{
		if(GPS_DMA_RecBuf[start] == '$')//search the first char '$'
		{
			if(((end-start)>= RMC_MAX_LEN) && ((memcmp(GPS_DMA_RecBuf+start+1, (const char *)("GNRMC"),5) == 0)))//find the frame
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
		if(GPS_DMA_RecBuf[start] == '\n')
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
/*
		nmea_parse(&parser, GNMRC_Buf, (cp_end-cp_st+1), &info);
	if(info.fix ==1)
		rt_kprintf("Fix not available\n");
	else if(info.fix ==2)
		rt_kprintf("2D available\n");
	else if(info.fix ==3)
		rt_kprintf("3D available\n");
	rt_kprintf("time:%d-%d-%d %d:%d:%d\n",info.utc.year,info.utc.mon, info.utc.day, info.utc.hour, info.utc.min, info.utc.sec);
	rt_kprintf("lat:%f,lon:%d\n",info.lat,info.lon);
    nmea_parser_destroy(&parser);
	*/
	//prase GNMRC
	
	
	
}

rt_uint8_t processdata(rt_uint8_t bufindex)
{
	rt_uint16_t start = 0;
	rt_uint16_t end = GPSBUFFERSIZE/2;
	rt_uint16_t cp_st, cp_end,temp=0;
	rt_uint16_t i;
	rt_int8_t tempbuf[GPSBUFFERSIZE/2];
	memcpy(tempbuf,GPS_DMA_RecBuf+bufindex*GPSBUFFERSIZE/2, GPSBUFFERSIZE/2);
	
	while(start< end)
	{
		if(tempbuf[start] == '$')//search the first char '$'
		{
			if(((end-start)>= RMC_MAX_LEN) && ((memcmp(tempbuf+start+1, (const char *)("GNRMC"),5) == 0)))//find the frame
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
		if(tempbuf[start] == '\n')
		{
			cp_end = start;
			break;
		}
		start++;
	}
	
	if(start == end)//faild
			return 0;
	memcpy(GNMRC_Buf,tempbuf+cp_st,cp_end-cp_st+1);

	rt_kprintf("index=%d\n",bufindex);//test
	for(i=0;i<cp_end-cp_st+1;i++)
		rt_kprintf("%c",GNMRC_Buf[i]);

	//prase GNMRC
	
}
#endif
void gps_thread_entry(void* parameter)
{
	rt_err_t  result;
	rt_uint8_t i;
	rt_uint16_t x;
rt_uint8_t MRC_Serach_flag = MRC_SEARCH_NEW;
	GPS_COM_Config();
	GPS_Data.vaild = 'V';
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
		/*
		 if(GPS_Buf_Flag & 0xf)//HT
		 {
		 	//for(x=0;x<512;x++)
			//	rt_kprintf("%c",GPS_DMA_RecBuf[x]);
			// rt_kprintf("recv half len\n");
			//i = processdata(0);
			GPS_Buf_Flag &=0xf0;
		 }
		 
		  if(GPS_Buf_Flag & 0xf0)//TC
		 {
		 //	for(x=512;x<1024;x++)
			//	rt_kprintf("%c",GPS_DMA_RecBuf[x]);
			//rt_kprintf("recv all len\n");
			//i = processdata(1);
			 i = processdata();
			GPS_Buf_Flag &=0x0f;
		 }
		 */
		 i = processdata();
		 //rt_sem_release(&gps_sem);
	}
}
#if 0
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
#endif
void GPS_DMA_RX_IRQHandler(void)
{
	static char count =0;
	/*
	if(DMA_GetITStatus(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_HTIF)==SET)
	{
		DMA_ClearITPendingBit(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_HTIF);
		GPS_Buf_Flag |= 0x1;
		//rt_sem_release(&gps_sem);
	}
	*/
	if(DMA_GetITStatus(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_TCIF)==SET)
	{
		DMA_ClearITPendingBit(GPS_RX_DMA_STREAM,GPS_RX_DMA_IT_TCIF);
		count++;
		if(count==10)
		{
			count=0;
			//GPS_Buf_Flag |= 0x10;
			rt_sem_release(&gps_sem);
		}		
	}
} 
