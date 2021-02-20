/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-19     luhuadong    first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "main"
#define DBG_LVL DBG_INFO
#include <rtdbg.h>

/* defined the LED0 pin: PE7 */
#define LED0_PIN        GET_PIN(E, 7)

#define PIN_LED_R       GET_PIN(E, 7)      // PE7 :  LED_R --> LED
#define PIN_LED_G       GET_PIN(E, 8)      // PE8 :  LED_B --> LED
#define PIN_LED_B       GET_PIN(E, 9)      // PE9 :  LED_G --> LED

/* 定义 LED 亮灭电平 */
#define LED_ON  (0)
#define LED_OFF (1)

/* 定义 8 组 LED 闪灯表，其顺序为 R G B */
static const rt_uint8_t _blink_tab[][3] = {
        { LED_ON,  LED_ON,  LED_ON  },
        { LED_OFF, LED_ON,  LED_ON  },
        { LED_ON,  LED_OFF, LED_ON  },
        { LED_ON,  LED_ON,  LED_OFF },
        { LED_OFF, LED_OFF, LED_ON  },
        { LED_ON,  LED_OFF, LED_OFF },
        { LED_OFF, LED_ON,  LED_OFF },
        { LED_OFF, LED_OFF, LED_OFF },
};

int main(void)
{
#if 0
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
#else
    unsigned int count = 1;
    unsigned int group_num = sizeof(_blink_tab) / sizeof(_blink_tab[0]);
    unsigned int group_current;

    /* 设置 RGB 灯引脚为输出模式 */
    rt_pin_mode(PIN_LED_R, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_LED_G, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_LED_B, PIN_MODE_OUTPUT);

    while (count > 0)
    {
        /* 获得组编号 */
        group_current = count % group_num;

        /* 控制 RGB 灯 */
        rt_pin_write(PIN_LED_R, _blink_tab[group_current][0]);
        rt_pin_write(PIN_LED_G, _blink_tab[group_current][1]);
        rt_pin_write(PIN_LED_B, _blink_tab[group_current][2]);

        /* 输出 LOG 信息 */
        LOG_D("group: %d | red led [%-3.3s] | green led [%-3.3s] | blue led [%-3.3s]", group_current,
                _blink_tab[group_current][0] == LED_ON ? "ON" : "OFF",
                _blink_tab[group_current][1] == LED_ON ? "ON" : "OFF",
                _blink_tab[group_current][2] == LED_ON ? "ON" : "OFF");

        /* 延时一段时间 */
        rt_thread_mdelay(500);
        count++;
    }
#endif

    return RT_EOK;
}

