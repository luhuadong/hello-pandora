/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-02-20     Rudy       the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#define DBG_TAG "motor"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

// KEY
#define PIN_KEY2        GET_PIN(D, 8)      // PD8 :  KEY2         --> KEY
#define PIN_KEY1        GET_PIN(D, 9)      // PD9 :  KEY1         --> KEY
#define PIN_KEY0        GET_PIN(D, 10)     // PD10:  KEY0         --> KEY
#define PIN_WK_UP       GET_PIN(C, 13)     // PC13:  WK_UP        --> KEY

// MOTOR
#define PIN_MOTOR_A     GET_PIN(A, 0)      // PA0 :  MOTOR_A      --> MOTOR
#define PIN_MOTOR_B     GET_PIN(A, 1)      // PA1 :  MOTOR_B      --> MOTOR

// BEEP
#define PIN_BEEP        GET_PIN(B, 2)      // PB2 :  BEEP         --> BEEP

enum
{
    MOTOR_STOP,
    MOTOR_LEFT,
    MOTOR_RIGHT
};

/* 电机控制 */
static void motor_ctrl(rt_uint8_t turn)
{
    if (turn == MOTOR_STOP)
    {
        rt_pin_write(PIN_MOTOR_A, PIN_LOW);
        rt_pin_write(PIN_MOTOR_B, PIN_LOW);
    }
    else if (turn == MOTOR_LEFT)
    {
        rt_pin_write(PIN_MOTOR_A, PIN_LOW);
        rt_pin_write(PIN_MOTOR_B, PIN_HIGH);
    }
    else if (turn == MOTOR_RIGHT)
    {
        rt_pin_write(PIN_MOTOR_A, PIN_HIGH);
        rt_pin_write(PIN_MOTOR_B, PIN_LOW);
    }
    else
    {
        LOG_D("err parameter ! Please enter 0-2.");
    }
}

static void beep_ctrl(rt_uint8_t on)
{
    if (on)
    {
        rt_pin_write(PIN_BEEP, PIN_HIGH);
    }
    else
    {
        rt_pin_write(PIN_BEEP, PIN_LOW);
    }
}

/* 中断回调 */
static void irq_callback(void *args)
{
    rt_uint32_t sign = (rt_uint32_t)args;

    switch (sign)
    {
    case PIN_KEY0:
        motor_ctrl(MOTOR_LEFT);
        LOG_D("KEY0 interrupt. motor turn left.");
        break;
    case PIN_KEY1:
        motor_ctrl(MOTOR_RIGHT);
        LOG_D("KEY1 interrupt. motor turn right.");
        break;
    case PIN_KEY2:
        motor_ctrl(MOTOR_STOP);
        LOG_D("KEY2 interrupt. motor stop.");
        break;
    default:
        LOG_E("error sign= %d !", sign);
        break;
    }
}

static int motor_sample(void)
{
    unsigned int count = 1;

    /* 设置按键引脚为输入模式 */
    rt_pin_mode(PIN_KEY0, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_KEY1, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_KEY2, PIN_MODE_INPUT_PULLUP);
    rt_pin_mode(PIN_WK_UP, PIN_MODE_INPUT_PULLDOWN);

    /* 设置电机控制引脚为输入模式 */
    rt_pin_mode(PIN_MOTOR_A, PIN_MODE_OUTPUT);
    rt_pin_mode(PIN_MOTOR_B, PIN_MODE_OUTPUT);

    /* 设置蜂鸣器引脚为输出模式 */
    rt_pin_mode(PIN_BEEP, PIN_MODE_OUTPUT);

    /* 设置按键中断模式与中断回调函数 */
    rt_pin_attach_irq(PIN_KEY0, PIN_IRQ_MODE_FALLING, irq_callback, (void *)PIN_KEY0);
    rt_pin_attach_irq(PIN_KEY1, PIN_IRQ_MODE_FALLING, irq_callback, (void *)PIN_KEY1);
    rt_pin_attach_irq(PIN_KEY2, PIN_IRQ_MODE_FALLING, irq_callback, (void *)PIN_KEY2);

    /* 使能中断 */
    rt_pin_irq_enable(PIN_KEY0, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(PIN_KEY1, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(PIN_KEY2, PIN_IRQ_ENABLE);

    while (count > 0)
    {
        if (rt_pin_read(PIN_WK_UP) == PIN_HIGH)
        {
            rt_thread_mdelay(50);
            if (rt_pin_read(PIN_WK_UP) == PIN_HIGH)
            {
                LOG_D("WK_UP pressed. beep on.");
                beep_ctrl(1);
            }
        }
        else
        {
            beep_ctrl(0);
        }
        rt_thread_mdelay(10);
        count++;
    }
    return 0;
}
INIT_APP_EXPORT(motor_sample);
