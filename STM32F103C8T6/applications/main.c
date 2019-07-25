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
#include "sensor.h"
#include "sensor_dallas_ds18b20.h"


static int rt_hw_ds18b20_port(void);

#define LED0                GET_PIN(C,13)
#define DS18B20_DATA_PIN    GET_PIN(B,9)

#define THREAD1_PRIORITY    5
#define THREAD1_STACK_SIZE  500

#define THREAD2_PRIORITY    10
#define THREAD2_STACK_SIZE  100

#define THREAD_TIMELICE     5

static struct rt_thread rt_thread2;
ALIGN(RT_ALIGN_SIZE)
static unsigned char rt_thred2_stack[1000];


static void thread2_entry(void * parameter)
{
    rt_pin_mode(LED0,PIN_MODE_OUTPUT);
    while(1)
    {			  
        rt_pin_write(LED0,PIN_HIGH);
       // rt_kprintf("led on \n");
        rt_thread_delay(500);
        rt_pin_write(LED0,PIN_LOW);
       // rt_kprintf("led off \n");
        rt_thread_delay(500);
		}
}

static void read_temp_entry(void *parameter)
{
    rt_device_t dev = RT_NULL;
    struct rt_sensor_data sensor_data;
    rt_size_t res;	
	 	rt_pin_mode(DS18B20_DATA_PIN ,PIN_MODE_OUTPUT);
	  #if 1
    dev = rt_device_find(parameter);
    if (dev == RT_NULL)
    {
        rt_kprintf("Can't find de//vice:%s\n", parameter);
        return;
    }
	  #endif
   
		#if 1
    if (rt_device_open(dev, RT_DEVICE_FLAG_RDWR) != RT_EOK)
    {
        rt_kprintf("open device failed!\n");
        return;
    }
    rt_device_control(dev, RT_SENSOR_CTRL_SET_ODR, (void *)100);
		#endif

    while (1)
    {
        res = rt_device_read(dev, 0, &sensor_data, 1);
        if (res != 1)
        {
            rt_kprintf("read data failed!size is %d\n", res);
            rt_device_close(dev);
            return;
        }
        else
        {
            rt_kprintf("temp:%3d.%dC, timestamp:%5d\n",
                       sensor_data.data.temp / 10,
                       sensor_data.data.temp % 10,
                       sensor_data.timestamp);
        }
        rt_thread_mdelay(100);
    }
}

static void thread_all_start(void)
{
    #if 1
  	rt_thread_t tid2 = RT_NULL;	
	  
	  tid2 = rt_thread_create("thread1",read_temp_entry,"temp_ds18b20",THREAD1_STACK_SIZE,THREAD1_PRIORITY,THREAD_TIMELICE);	 
	  if(tid2 != RT_NULL)
    {
        rt_thread_startup(tid2);
    }
		else
		{
		    rt_kprintf("thread1 creat failed \n");
		}
	  #endif
	
	  #if 0
    rt_thread_init(&rt_thread2,"led_thread",thread2_entry,RT_NULL,&rt_thred2_stack[0],sizeof(rt_thred2_stack),THREAD2_PRIORITY,THREAD_TIMELICE);
    rt_thread_startup(&rt_thread2);   
	  #endif
		
}
MSH_CMD_EXPORT(thread_all_start,"thread_test");

int main(void)
{
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    thread_all_start();
}


static int rt_hw_ds18b20_port(void)
{
    struct rt_sensor_config cfg;
    
    cfg.intf.user_data = (void *)DS18B20_DATA_PIN;
    rt_hw_ds18b20_init("ds18b20", &cfg);
    
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_ds18b20_port);

















