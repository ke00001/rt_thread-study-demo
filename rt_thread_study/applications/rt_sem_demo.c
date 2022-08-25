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
#define DBG_TAG "RT_sem_demo"
#define DBG_LVL DBG_LOG
#include "rt_demo.h"
#ifdef RT_SEM_DEMO
/* 定义信号量控制块 */
static rt_sem_t test_sem = RT_NULL;

static rt_thread_t send_sem_thread = RT_NULL;
static rt_thread_t recv_sem_thread = RT_NULL;

uint8_t ucValue[2] = {0};
void test_sem_send_thread(void *par)
{
    while(1)
    {
        rt_sem_take(test_sem, RT_WAITING_FOREVER);
        ucValue[0]++;
        rt_thread_mdelay(50);
        ucValue[1]++;
        rt_sem_release(test_sem);
        rt_thread_yield(); //放弃剩余时间片，直接调度

    }
}

void test_sem_recv_thread(void *par)
{
    while(1)
    {
        rt_sem_take(test_sem, RT_WAITING_FOREVER);
        if (ucValue[0]==ucValue[1] )
        {
            rt_kprintf("rt_sem_demo success!\r\n");
        }
        else {
            rt_kprintf("rt_sem_demo fail!\r\n");
        }
        rt_sem_release(test_sem);
        rt_thread_mdelay(3000);
    }
}



 void  rt_sem_demo_thread_entry(void *par)
{
    /*生成二值信号量*/
    test_sem = rt_sem_create("sem_demo", 1, RT_IPC_FLAG_PRIO);
    if(test_sem != RT_NULL)
    {
        rt_kprintf("test_sem 创建成功\r\n");
    }
    else {
        rt_kprintf("test_sem creat fail! ");
    }
    send_sem_thread = rt_thread_create("send_sem", test_sem_send_thread, RT_NULL, 512, 8, 20);
    if(send_sem_thread != RT_NULL)
    {
        rt_thread_startup(send_sem_thread);
    }
    else {
        rt_kprintf("send_sem_thread creat fail! ");
    }

    recv_sem_thread = rt_thread_create("recv_sem", test_sem_recv_thread, RT_NULL, 512, 8, 20);
    if(recv_sem_thread != RT_NULL)
    {
        rt_thread_startup(recv_sem_thread);
    }
    else {
        rt_kprintf("send_sem_thread creat fail! ");
    }

}
#endif
