#ifndef _GPS_H_
#define _GPS_H_
#include <rtthread.h>
#include "stm32f4xx.h"

#define ARRAY_LEN(x)            (sizeof(x) / sizeof((x)[0]))


#define GPS_COM                        UART5
#define GPS_COM_CLK                    RCC_APB1Periph_UART5
#define GPS_COM_TX_PIN                 GPIO_Pin_12
#define GPS_COM_TX_GPIO_PORT           GPIOC
#define GPS_COM_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define GPS_COM_TX_SOURCE              GPIO_PinSource12
#define GPS_COM_TX_AF                  GPIO_AF_UART5
#define GPS_COM_RX_PIN                 GPIO_Pin_2
#define GPS_COM_RX_GPIO_PORT           GPIOD
#define GPS_COM_RX_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define GPS_COM_RX_SOURCE              GPIO_PinSource2
#define GPS_COM_RX_AF                  GPIO_AF_UART5
#define GPS_COM_IRQn                   UART5_IRQn
#define GPS_COM_ISR										 UART5_IRQHandler
#define GPS_COM_RST_GPIO_CLK           	 RCC_AHB1Periph_GPIOB
#define GPS_COM_RST_PIN								 GPIO_Pin_4
#define GPS_COM_RST_PORT							   GPIOB
#define GPS_COM_1PPS_GPIO_CLK           	 RCC_AHB1Periph_GPIOB
#define GPS_COM_1PSS_PIN								 GPIO_Pin_5
#define GPS_COM_1PPS_PORT							   GPIOB


/* Definition for DMAx resources ********************************************/
#define GPS_DR_ADDRESS				 ((uint32_t)UART5 + 0x04) 

#define GPS_DMA						 DMA1
#define GPS_DMA_CLK 				 RCC_AHB1Periph_DMA1
   
#define GPS_TX_DMA_CHANNEL			 DMA_Channel_4
#define GPS_TX_DMA_STREAM			 DMA1_Stream7
#define GPS_TX_DMA_FLAG_FEIF 		 DMA_FLAG_FEIF7
#define GPS_TX_DMA_FLAG_DMEIF		 DMA_FLAG_DMEIF7
#define GPS_TX_DMA_FLAG_TEIF 		 DMA_FLAG_TEIF7
#define GPS_TX_DMA_FLAG_HTIF 		 DMA_FLAG_HTIF7
#define GPS_TX_DMA_FLAG_TCIF 		 DMA_FLAG_TCIF7
			
#define GPS_RX_DMA_CHANNEL			 DMA_Channel_4
#define GPS_RX_DMA_STREAM			 DMA1_Stream0
#define GPS_RX_DMA_FLAG_FEIF 		 DMA_FLAG_FEIF0
#define GPS_RX_DMA_FLAG_DMEIF		 DMA_FLAG_DMEIF0
#define GPS_RX_DMA_FLAG_TEIF 		 DMA_FLAG_TEIF0
#define GPS_RX_DMA_FLAG_HTIF 		 DMA_FLAG_HTIF0
#define GPS_RX_DMA_FLAG_TCIF 		 DMA_FLAG_TCIF0

#define GPS_RX_DMA_IT_FEIF 		 	 DMA_FLAG_FEIF0
#define GPS_RX_DMA_IT_DMEIF		 	DMA_IT_DMEIF0
#define GPS_RX_DMA_IT_TEIF 		 	DMA_IT_TEIF0
#define GPS_RX_DMA_IT_HTIF 		 	DMA_IT_HTIF0
#define GPS_RX_DMA_IT_TCIF 		 	DMA_IT_TCIF0


#define GPS_DMA_TX_IRQn				 DMA1_Stream7_IRQn
#define GPS_DMA_RX_IRQn				 DMA1_Stream0_IRQn
#define GPS_DMA_TX_IRQHandler		 DMA1_Stream7_IRQHandler
#define GPS_DMA_RX_IRQHandler		 DMA1_Stream0_IRQHandler

#define GPSBUFFERSIZE 1024
#define RMC_MAX_LEN		(70+5)
#define MRC_SEARCH_NEW		0
#define MRC_SEARCH_MEXT		1

typedef struct{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}UTC_TIME;
typedef struct{
	unsigned int year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}LOCAL_TIME;
typedef struct{
	unsigned char vaild;//'A'定位有效，'V‘定位无效
	UTC_TIME utc_time;//UTC时间
	LOCAL_TIME local_time;//本地时间
	double latitude;//纬度，单位为度，正数表示北纬，负数表示南纬
	double longitude;//经度，单位为度，正数表示东经，负数表示西经
	
}GPS_INFO;


extern rt_thread_t gps_thread;

static void GPS_COM_Config(void);
void gps_thread_entry(void* parameter);
rt_uint8_t processdata(/*rt_uint8_t bufindex*/void);
unsigned char CalcCheck(char* Bytes,int len);
char calculateTimezone(double lat,double lon);
void UTCTOLocalTime(char timezone,UTC_TIME utc_time,LOCAL_TIME * local_time);

#endif





