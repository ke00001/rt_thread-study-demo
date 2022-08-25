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
#include "bsp_h/bsp_key.h"

#define DBG_TAG "bsp_key"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

int  bsp_key_init(void)
{
    rt_pin_mode( KEY1_PIN,PIN_MODE_INPUT );
    rt_pin_mode( KEY2_PIN,PIN_MODE_INPUT );
    return RT_EOK;
}
INIT_BOARD_EXPORT(bsp_key_init);

