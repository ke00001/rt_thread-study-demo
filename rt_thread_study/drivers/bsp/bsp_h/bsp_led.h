/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-07-04     wangn       the first version
 */
#ifndef APPLICATIONS_BSP_LED_H_
#define APPLICATIONS_BSP__LED_H_

#define LED_R_PIN  GET_PIN(B,5)
#define LED_G_PIN  GET_PIN(B,0)
#define LED_B_PIN  GET_PIN(B,1)

#define LED_ON( led_pin)  rt_pin_write(led_pin,PIN_HIGH)
#define LED_OFF(led_pin) rt_pin_write(led_pin,PIN_LOW)


void led_demo_thread_entry(void *par);
#endif /* APPLICATIONS_BSP_LED_H_ */
