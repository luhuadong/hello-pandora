/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2019-08-28     WillianChan   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#ifdef BSP_USING_SPI_LCD
#include <drv_lcd.h>
#include <rttlogo.h>
#include <logo.h>

static int lcd_sample(void)
{
    /* clear screen */
    lcd_clear(WHITE);

    /* show RT-Thread logo */
    //lcd_show_image(0, 0, 240, 69, image_rttlogo);
    lcd_show_image(0, 0, 240, 240, gImage_0wastehome);
    
    /* set background and foreground color */
    lcd_set_color(WHITE, BLACK);

    /* show string */
    //lcd_show_string(10, 69, 16, "Hello, Bocon!");
    //lcd_show_string(10, 69+16, 24, "Bocon");
    //lcd_show_string(10, 69+16+24, 32, "Bocon");
    
    /* draw a line */
    //lcd_draw_line(0, 69+16+24+32, 240, 69+16+24+32);
    
    /* draw some circles */
    /*

    lcd_draw_point(120, 194);
    for (int i = 0; i < 46; i += 4)
    {
        lcd_draw_circle(120, 194, i);
    }
    */
    
    return RT_EOK;
}
INIT_APP_EXPORT(lcd_sample);
#endif /* BSP_USING_SPI_LCD */
