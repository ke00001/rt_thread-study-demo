/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-24     wangn       the first version
 */
#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>
#include "rt_demo.h"

/*
    当中断产生时，处理机将按如下的顺序执行：
    1. 保存当前处理机状态信息
    2. 载入异常或中断处理函数到 PC 寄存器
    3. 把控制权转交给处理函数并开始执行
    4. 当处理函数执行完成时，恢复处理器状态信息
    5. 从异常或中断中返回到前一个程序执行点

    中断发生的环境有两种情况：在线程的上下文中，在中断服务函数处理上下文中。
   1.  线程在工作的时候，如果此时发生了一个中断，无论中断的优先级是多大，都会
                   打断当前线程的执行，从而转到对应的中断服务函数中执行

     2. 在执行中断服务例程的过程中，如果有更高优先级别的中断源触发中断，由于当
                前处于中断处理上下文环境中，根据不同的处理器构架可能有不同的处理方式比
                如新的中断等待挂起直到当前中断处理离开后再行响应；或新的高优先级中断
                打断当前中断处理过程，而去直接响应这个更高优先级的新中断源。后面这种情
                况，称之为中断嵌套。在硬实时环境中，前一种情况是不允许发生的，不能使响
                应中断的时间尽量的短。而在软件处理（软实时环境） 上， RT-Thread 允许中断
                嵌套，即在一个中断服务例程期间，处理器可以响应另外一个优先级更高的中断。
*/
#define DBG_TAG "rt_nvic_demo"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "bsp/bsp_h/bsp_key.h"

#ifdef RT_NVIC_DEMO
rt_mq_t nvic_demo_mq = RT_NULL;
rt_sem_t nvic_demo_sem = RT_NULL;

typedef struct
{
    char *name;
    uint32_t count ;
    uint32_t time_l;
    uint32_t use;
}isr_hanndle_s;
isr_hanndle_s key1_date = {.name = "key1",.count = 0,.time_l = 0,.use = !RT_EOK};
isr_hanndle_s key2_date = {.name = "key2",.count = 0,.time_l = 0,.use = !RT_EOK};

static void key1_isr(void *par)
{
    uint32_t send_data = 1;
    isr_hanndle_s *data = par;
    data->count++;
    if( (rt_tick_get() < data->time_l+10) && (0xFFFFFFFF-data->time_l>10) )
        data->use = !RT_EOK;
    else
    {
        data->use = RT_EOK;
        data->time_l = rt_tick_get() ;
    }

    rt_mq_send(nvic_demo_mq, &send_data, sizeof(send_data));

}

static void key2_isr(void *par)
{
    uint32_t send_data = 2;
    isr_hanndle_s *data = par;
    data->count++;
    if( (rt_tick_get() < data->time_l+10) && (0xFFFFFFFF-data->time_l>10) )
        data->use = !RT_EOK;
    else
    {
        data->use = RT_EOK;
        data->time_l = rt_tick_get() ;
    }
    rt_mq_send(nvic_demo_mq, &send_data, sizeof(send_data));
}

static void key_nvic_thread_entry(void *par)
{
    uint32_t r_queue = 0;
    rt_err_t err = RT_EOK;

    rt_pin_attach_irq(KEY1_PIN, PIN_IRQ_MODE_RISING, key1_isr  , &key1_date);
    rt_pin_attach_irq(KEY2_PIN, PIN_IRQ_MODE_RISING, key2_isr  , &key2_date);
    rt_pin_irq_enable(KEY1_PIN, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY2_PIN, PIN_IRQ_ENABLE);
    while(1)
    {
        err = rt_mq_recv(nvic_demo_mq, &r_queue, sizeof(r_queue), RT_WAITING_FOREVER);
        if(err == RT_EOK)
        {

            if(key1_date.use == RT_EOK )
            {
                key1_date.use = !RT_EOK;
                LOG_I("收到KEY1消息 %d",r_queue);
            }
            if(key2_date.use == RT_EOK )
            {
                key2_date.use = !RT_EOK;
                LOG_I("收到KEY2消息 %d",r_queue);
            }
            rt_thread_mdelay(10);
        }
    }

}








void rt_nvic_demo_thread_entry(void *par)
{
    static rt_thread_t  key_nvic_thread = RT_NULL;

    /* 创建一个消息队列 */
    nvic_demo_mq = rt_mq_create("nvic_demo_mq", /* 消息队列名字 */
                                4,              /* 消息的最大长度 */
                                2,              /* 消息队列的最大容量 */
                                RT_IPC_FLAG_PRIO );    /* 队列模式PRIO(0x01)*/
    if (nvic_demo_mq == RT_NULL)
        LOG_I("消息队列创建失败！ ");

    /* 创建一个信号量 */
    nvic_demo_sem = rt_sem_create("nvic_demo_sem",      /* 消息队列名字 */
                                    0,                  /* 信号量初始值，默认有一个信号量 */
                                    RT_IPC_FLAG_PRIO);  /* 信号量模式 FIFO(0x00)*/
    if (nvic_demo_sem == RT_NULL)
        LOG_I("信号量创建失败！ ");

    key_nvic_thread = rt_thread_create("key_nvic_thread", key_nvic_thread_entry, RT_NULL, 512, 8, 20);
    if (key_nvic_thread != RT_NULL)
        rt_thread_startup(key_nvic_thread);
    else LOG_I("key_nvic_thread create error  ");


}
#endif


















