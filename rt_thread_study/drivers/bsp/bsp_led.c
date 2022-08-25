/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-04     wangn       the first version
 */

#include "board.h"
#include<rtthread.h>
#include<rtdevice.h>
#include "bsp/bsp_h/bsp_led.h"

#define DBG_TAG "bsp_led"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int led_init(void)
{
    rt_pin_mode( LED_R_PIN,PIN_MODE_OUTPUT_OD );
    rt_pin_mode( LED_G_PIN,PIN_MODE_OUTPUT_OD );
    rt_pin_mode( LED_B_PIN,PIN_MODE_OUTPUT_OD );
    rt_pin_write(LED_R_PIN,PIN_HIGH);
    rt_pin_write(LED_G_PIN,PIN_HIGH);
    rt_pin_write(LED_B_PIN,PIN_HIGH);
    return RT_EOK;
}
INIT_BOARD_EXPORT(led_init);




/* 定义线程控制块 */
  static struct rt_thread led_g_thread;
  /* 定义线程控栈时要求 RT_ALIGN_SIZE 个字节对齐 */
  ALIGN(RT_ALIGN_SIZE)   static rt_uint8_t rt_led_g_thread_stack[1024];

  static void led_g_thread_entry(void* parameter)
{
    while(1)
    {
        LED_ON(LED_G_PIN);
        rt_thread_delay(500);
        LED_OFF(LED_G_PIN);
        rt_thread_delay(500);


    }
}





static void led_r_thread_entry(void *parr)
{
    while(1)
    {
        LED_ON(LED_R_PIN);
        rt_thread_delay(500);
        LED_OFF(LED_R_PIN);
        rt_thread_delay(500);

    }

}

static void led_b_thread_entry(void *parr)
{
    while(1)
    {
        LED_ON(LED_B_PIN);
        rt_thread_delay(500);
        LED_OFF(LED_B_PIN);
        rt_thread_delay(500);

    }

}

static rt_thread_t rt_led_r_thread= RT_NULL;
static rt_thread_t rt_led_b_thread= RT_NULL;



void led_demo_thread_entry(void *par)
{

    rt_led_r_thread =  rt_thread_create(  "led_r", led_r_thread_entry,RT_NULL,512,7,20);
    /* 启动线程，开启调度 */
    if (rt_led_r_thread != RT_NULL)
        rt_thread_startup(rt_led_r_thread);
    else
        rt_kprintf("led_demo error\r\n");

    rt_thread_delay(191);

    rt_thread_init( &led_g_thread,"led_g",led_g_thread_entry,RT_NULL,rt_led_g_thread_stack,sizeof(rt_led_g_thread_stack),7,20);
    rt_thread_startup(&led_g_thread);

    rt_thread_delay(191);

    rt_led_b_thread =  rt_thread_create(  "led_b", led_b_thread_entry,RT_NULL,512,7,20);
   /* 启动线程，开启调度 */
    if (rt_led_b_thread != RT_NULL)
       rt_thread_startup(rt_led_b_thread);
    else
       rt_kprintf("led_demo error\r\n");
}

