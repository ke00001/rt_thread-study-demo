/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-10     wangn       the first version
 */
#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>
#define DBG_TAG "rt_event_demo"
#define DBG_LVL DBG_LOG
#include "bsp/bsp_h/bsp_key.h"
#include "rt_demo.h"
#ifdef RT_EVENT_DEMO

#define EVENT_KEY1  (0X01<<0)
#define EVENT_KEY2  (0X01<<1)


rt_thread_t recv_event_thread = RT_NULL;
rt_thread_t send_event_thread = RT_NULL;
rt_event_t test_event = RT_NULL;


static void recv_event_thread_entry(void *par)
{
    rt_uint32_t recv_flag = 0;
    while(1)
    {
        rt_event_recv(test_event, EVENT_KEY1|EVENT_KEY2, RT_EVENT_FLAG_AND|RT_EVENT_FLAG_CLEAR, RT_WAITING_FOREVER, &recv_flag);
        switch(recv_flag)
        {
        case EVENT_KEY1:
            rt_kprintf("key1 被按下\r\n");
            break;
        case EVENT_KEY2:
            rt_kprintf("key2 被按下\r\n");
            break;
        case EVENT_KEY1|EVENT_KEY2:
            rt_kprintf("key1 和 key2 被按下\r\n");
            break;
        }
    }
}


static void send_event_thread_entry(void *par)
{

    while(1)
    {

        if(rt_pin_read(KEY1_PIN))
        {
            rt_thread_mdelay(10);
            if(rt_pin_read(KEY1_PIN))
                rt_event_send(test_event, EVENT_KEY1);
        }
        if(rt_pin_read(KEY2_PIN))
        {
            rt_thread_mdelay(10);
            if(rt_pin_read(KEY2_PIN))
            rt_event_send(test_event, EVENT_KEY2);
        }
        rt_thread_mdelay(100);
    }
}


void rt_event_demo_thread_entry(void *par)
{
    test_event = rt_event_create("event_demo", RT_IPC_FLAG_FIFO);
    if(test_event!= RT_NULL )
    {
        rt_kprintf("事件创建成功\r\n");
    }

    recv_event_thread = rt_thread_create("event_recv", recv_event_thread_entry, RT_NULL, 512, 8, 20);
    if(recv_event_thread != RT_NULL )
    {
        rt_thread_startup(recv_event_thread);
        rt_kprintf("recv_event_thread create success\r\n");
    }

    send_event_thread = rt_thread_create("event_send", send_event_thread_entry, RT_NULL, 512, 8, 20);
    if(send_event_thread != RT_NULL )
    {
        rt_thread_startup(send_event_thread);
        rt_kprintf("recv_event_thread create success\r\n");
    }
}
#endif

