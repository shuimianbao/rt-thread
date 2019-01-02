/*
 * File      : application.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006, RT-Thread Development Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      the first version
 * 2014-04-27     Bernard      make code cleanup. 
 */

#include <board.h>
#include <rtthread.h>
#include "systemled.h"
#include "rtc.h"
#include "adc.h"
#include "extio.h"
#include "eeprom.h"
#include "lte.h"
#include "gps.h"
#include "rs485.h"

#ifdef RT_USING_FINSH
#include <shell.h>
#include <finsh.h>
#endif

#ifdef RT_USING_GDB
#include <gdb_stub.h>
#endif


void rt_init_thread_entry(void* parameter)
{
    /* initialization RT-Thread Components */
    rt_components_init();
	
		/* Create led thread */
    systemled_thread = rt_thread_create("sys_led",
    		systemled_thread_entry, RT_NULL,
    		256, 20, 20);
    if(systemled_thread != RT_NULL)
    	rt_thread_startup(systemled_thread);
		
		/* Create rtc thread */
    rtc_thread = rt_thread_create("rtc",
    		rtc_thread_entry, RT_NULL,
    		256, 20, 20);
    if(rtc_thread != RT_NULL)
    	rt_thread_startup(rtc_thread);
		
		/* Create adc thread */
    adc_thread = rt_thread_create("adc",
    		adc_thread_entry, RT_NULL,
    		256, 20, 20);
    if(adc_thread != RT_NULL)
    	rt_thread_startup(adc_thread);
		
		/* Create external output thread */
    extio_thread = rt_thread_create("ext_io",
    		extio_thread_entry, RT_NULL,
    		256, 20, 20);
    if(extio_thread != RT_NULL)
    	rt_thread_startup(extio_thread);
		
		/* Create eeprom thread */
    eeprom_thread = rt_thread_create("eeprom",
    		eeprom_thread_entry, RT_NULL,
    		256, 20, 20);
    if(eeprom_thread != RT_NULL)
    	rt_thread_startup(eeprom_thread);
		
		/* Create gps thread */
    gps_thread = rt_thread_create("gps",
    		gps_thread_entry, RT_NULL,
    		256, 20, 20);
    if(gps_thread != RT_NULL)
    	rt_thread_startup(gps_thread);
		
		/* Create lte thread */
    lte_thread = rt_thread_create("lte",
    		lte_thread_entry, RT_NULL,
    		256, 20, 20);
    if(lte_thread != RT_NULL)
    	rt_thread_startup(lte_thread);
		
		/* Create rs485 thread */
    rs485_thread = rt_thread_create("rs485",
    		RS485_thread_entry, RT_NULL,
    		256, 20, 20);
    if(rs485_thread != RT_NULL)
    	rt_thread_startup(rs485_thread);
    /* GDB STUB */
#ifdef RT_USING_GDB
    gdb_set_device("uart6");
    gdb_start();
#endif
}

int rt_application_init()
{
    rt_thread_t tid;

    tid = rt_thread_create("init",
        rt_init_thread_entry, RT_NULL,
        2048, RT_THREAD_PRIORITY_MAX/3, 20);

    if (tid != RT_NULL)
        rt_thread_startup(tid);

    return 0;
}
