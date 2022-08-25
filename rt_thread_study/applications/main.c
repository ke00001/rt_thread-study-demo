/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-04     RT-Thread    first version
 */

#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "bsp/bsp_h/bsp_led.h"
#include "rt_demo.h"


#ifdef RT_LED_DEMO
/* led demo */
rt_thread_t rt_led_demo_thread = RT_NULL;
#endif

#ifdef RT_MQ_DEMO
/* 邮箱demo */
extern void rt_mq_demo_thread_entry(void *par);
rt_thread_t rt_mq_demo_thread = RT_NULL;
#endif

#ifdef RT_SEM_DEMO
extern void  rt_sem_demo_thread_entry(int *par);
rt_thread_t rt_sem_demo_thread = RT_NULL;
#endif

#ifdef RT_MUTEX_DEMO
extern void rt_mutex_demo_thread_entry(void *par);
rt_thread_t rt_mutex_demo_thread = RT_NULL;

#endif

#ifdef RT_EVENT_DEMO
extern void rt_event_demo_thread_entry(void *par);
rt_thread_t rt_event_demo_thread = RT_NULL;
#endif

extern void rt_nvic_demo_thread_entry(void *par);
rt_thread_t rt_nvic_demo_thread = RT_NULL;

int main(void)
{
    /* led demo */
    #ifdef RT_LED_DEMO

    rt_led_demo_thread = rt_thread_create (" rt_mq_demo",led_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_led_demo_thread != RT_NULL)
        rt_thread_startup(rt_led_demo_thread);
    else rt_kprintf("rt_led_demo_thread create  error");
#endif

    /* 邮箱demo */
#ifdef RT_MQ_DEMO
    rt_mq_demo_thread = rt_thread_create (" rt_mq_demo",rt_mq_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_mq_demo_thread != RT_NULL)
        rt_thread_startup(rt_mq_demo_thread);
    else rt_kprintf("rt_mq_demo_thread create error");
#endif

#ifdef RT_SEM_DEMO
    rt_sem_demo_thread = rt_thread_create (" rt_sem_demo",rt_sem_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_sem_demo_thread != RT_NULL)
        rt_thread_startup(rt_sem_demo_thread);
    else rt_kprintf("rt_sem_demo_thread create error");
#endif


#ifdef RT_MUTEX_DEMO
    rt_mutex_demo_thread = rt_thread_create (" rt_mutex_demo",rt_mutex_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_mutex_demo_thread != RT_NULL)
        rt_thread_startup(rt_mutex_demo_thread);
    else rt_kprintf("rt_mutex_demo_thread create error");
#endif

#ifdef RT_EVENT_DEMO
    rt_event_demo_thread = rt_thread_create (" rt_event_demo",rt_event_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_event_demo_thread != RT_NULL)
        rt_thread_startup(rt_event_demo_thread);
    else rt_kprintf("rt_event_demo_thread create error");
#endif

#ifdef RT_NVIC_DEMO
    rt_nvic_demo_thread = rt_thread_create (" rt_nvic_demo",rt_nvic_demo_thread_entry,RT_NULL,512,8,20);
    if (rt_nvic_demo_thread != RT_NULL)
        rt_thread_startup(rt_nvic_demo_thread);
    else rt_kprintf("rt_nvic_demo_thread create error");
#endif

    return RT_EOK;
}






