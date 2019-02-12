#ifndef _LCD_H_
#define _LCD_H_
#include <rtthread.h>
#include "stm32f4xx.h"

extern rt_thread_t lcd_thread;

void lcd_thread_entry(void* parameter);

#endif

