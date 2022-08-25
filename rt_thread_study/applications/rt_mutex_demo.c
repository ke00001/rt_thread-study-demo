/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-10     wangn       the first version
 */

/*
 *  使用互斥量时候需要注意几点：
 * 1. 两个线程不能对同时持有同一个互斥量。如果某线程对已被持有的互斥量进行获取，则该线程会被挂起，
 *          直到持有该互斥量的线程将互斥量释放成功， 其他线程才能申请这个互斥量。
 *
 * 2. 互斥量不能在中断服务程序中使用。
 *
 * 3. RT-Thread 作为实时操作系统需要保证线程调度的实时性，尽量避免线程的长时间阻塞，
 *          因此在获得互斥量之后，应该尽快释放互斥量。
 *
 *  4. 持有互斥量的过程中，不得再调用 rt_thread_control()等函数接口更改持有互斥量
 *线程的优先级
*/
#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>
#define DBG_TAG "rt_mutex_demo"
#define DBG_LVL DBG_LOG
#include "rt_demo.h"

#ifdef RT_MUTEX_DEMO
static rt_mutex_t test_mutex = RT_NULL; //必须定义成全局变量
static rt_thread_t recv_thread = RT_NULL;
static rt_thread_t send_thread = RT_NULL;
static uint8_t ucValue[2] = {0};



static void send_mutex_thread_entry(void *par)
{


    while(1)
    {
         rt_mutex_take(test_mutex, RT_WAITING_FOREVER);
         ucValue[0]++;
         rt_thread_mdelay(1000);
         rt_mutex_take(test_mutex, RT_WAITING_FOREVER);
         ucValue[1]++;
         rt_mutex_release(test_mutex);
         rt_mutex_release(test_mutex);
         rt_thread_yield();
    }
}

static void recv_mutex_thread_entry(void *par)
{
    while(1)
    {
        rt_mutex_take(test_mutex, RT_WAITING_FOREVER);
        if (ucValue[0]==ucValue[1] )
        {
            rt_kprintf("rt_mutex_demo success!\r\n");
        }
        else {
            rt_kprintf("rt_mutex_demo fail!\r\n");
        }
        rt_mutex_release(test_mutex);
        rt_thread_mdelay(3000);
    }

}

void rt_mutex_demo_thread_entry(void *par)
{
    test_mutex = rt_mutex_create("test_mutex", RT_IPC_FLAG_PRIO);
    if(test_mutex != RT_NULL)
    {
        rt_kprintf(" 互斥量创建成功\r\n ");
    }
    send_thread = rt_thread_create("send_mutexx", send_mutex_thread_entry, RT_NULL, 512, 8, 20);
     if (send_thread != RT_NULL)
     {
         rt_thread_startup(send_thread);
     }
     else {
         rt_kprintf("send_thread err");
     }

     recv_thread = rt_thread_create("recv_mutex", recv_mutex_thread_entry, RT_NULL, 512, 8, 20);
    if (recv_thread != RT_NULL)
    {
        rt_thread_startup(recv_thread);
    }
    else{
        rt_kprintf("recv_thread creat fail");
    }



}
#endif
