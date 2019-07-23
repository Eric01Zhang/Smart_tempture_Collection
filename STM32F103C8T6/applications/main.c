/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-03-08     obito0   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define LED0     GET_PIN(C,13)

#define THREAD1_PRIORITY    15
#define THREAD1_STACK_SIZE  500

#define THREAD2_PRIORITY    14
#define THREAD2_STACK_SIZE  100

#define THREAD_TIMELICE     5

static rt_thread_t tid1 = RT_NULL;
static struct rt_thread rt_thread2;

ALIGN(RT_ALIGN_SIZE)
static unsigned char rt_thred2_stack[1000];

static void thread1_entry(void *parameter)
{
    rt_uint32_t count = 0;

    while(1)
    {
        rt_kprintf("rt_thread count%d\n",count++);
        rt_thread_delay(500);
    }
}

static void thread2_entry(void * parameter)
{
    rt_pin_mode(LED0,PIN_MODE_OUTPUT);
    while(1)
    {
        rt_pin_write(LED0,PIN_HIGH);
        rt_kprintf("led on \n");
        rt_thread_delay(500);
        rt_pin_write(LED0,PIN_LOW);
        rt_kprintf("led off \n");
        rt_thread_delay(500);
    }
}

static void thread_all_start(void)
{
    tid1 = rt_thread_create("thread1",thread1_entry,RT_NULL,THREAD1_STACK_SIZE,THREAD1_PRIORITY,THREAD_TIMELICE);
    if(tid1 != RT_NULL)
    {
        rt_thread_startup(tid1);
    }

    #if 1
    rt_thread_init(&rt_thread2,"led_thread",thread2_entry,RT_NULL,&rt_thred2_stack[0],sizeof(rt_thred2_stack),THREAD2_PRIORITY,THREAD_TIMELICE);
    rt_thread_startup(&rt_thread2);   
	  #endif
		
}
MSH_CMD_EXPORT(thread_all_start,"thread_test");

int main(void)
{
    thread_all_start();
}


