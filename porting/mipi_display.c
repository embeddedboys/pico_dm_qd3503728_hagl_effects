/*

MIT License

Copyright (c) 2019-2023 Mika Tuupola
Copyright (c) 2025 embeddedboys

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

This file is part of the Raspberry Pi Pico MIPI DCS backend for the HAGL
graphics library: https://github.com/tuupola/hagl_pico_mipi

SPDX-License-Identifier: MIT

*/

#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
// #include <stdatomic.h>

#include <hardware/gpio.h>
#include <hardware/clocks.h>
#include <pico/time.h>

#include "mipi_dcs.h"
#include "mipi_display.h"

#include "ili9488.h"

static inline uint16_t
htons(uint16_t i)
{
    __asm ("rev16 %0, %0" : "+l" (i) : : );
    return i;
}

void
mipi_display_init()
{
#ifdef HAGL_HAL_USE_SINGLE_BUFFER
    hagl_hal_debug("%s\n", "Initialising single buffered display.");
#endif /* HAGL_HAL_USE_SINGLE_BUFFER */

#ifdef HAGL_HAL_USE_DOUBLE_BUFFER
    hagl_hal_debug("%s\n", "Initialising double buffered display.");
#endif /* HAGL_HAL_USE_DOUBLE_BUFFER */

#ifdef HAGL_HAL_USE_TRIPLE_BUFFER
    hagl_hal_debug("%s\n", "Initialising triple buffered display.");
#endif /* HAGL_HAL_USE_DOUBLE_BUFFER */

    ili9488_driver_init();

    uint32_t peri = clock_get_hz(clk_peri);
    uint32_t sys = clock_get_hz(clk_sys);
    hagl_hal_debug("clk_peri %d.\n", peri);
    hagl_hal_debug("clk_sys %d.\n", sys);
}

size_t
mipi_display_fill_xywh(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, void *_color)
{
    if (0 == w || 0 == h) {
        return 0;
    }

    x1 = x1 + MIPI_DISPLAY_OFFSET_X;
    y1 = y1 + MIPI_DISPLAY_OFFSET_Y;

    int32_t x2 = x1 + w - 1;
    int32_t y2 = y1 + h - 1;
    size_t size = w * h;
    uint16_t *color = _color;

    ili9488_fill_color(x1, y1, x2, y2, htons(*color), size);

    return size;
}

size_t
mipi_display_write_xywh(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint8_t *buffer)
{
    if (0 == w || 0 == h) {
        return 0;
    }

    x1 = x1 + MIPI_DISPLAY_OFFSET_X;
    y1 = y1 + MIPI_DISPLAY_OFFSET_Y;

    int32_t x2 = x1 + w - 1;
    int32_t y2 = y1 + h - 1;
    uint32_t size = w * h;
    uint16_t *color = (uint16_t *)buffer;

    for (size_t i = 0; i < size; i++)
        color[i] = htons(color[i]);

    ili9488_video_flush(x1, y1, x2, y2, (void *)buffer, size);

    return size * MIPI_DISPLAY_DEPTH / 8;
}

size_t
mipi_display_write_xy(uint16_t x1, uint16_t y1, uint8_t *buffer)
{
    uint16_t color = htons(*(uint16_t *)buffer);

    x1 = x1 + MIPI_DISPLAY_OFFSET_X;
    y1 = y1 + MIPI_DISPLAY_OFFSET_Y;

    ili9488_video_flush(x1, y1, x1, y1, &color, 1);

    /* This should also include the bytes for writing the commands. */
    return MIPI_DISPLAY_DEPTH / 8;
}

// /* TODO: This most likely does not work with dma atm. */
// void
// mipi_display_ioctl(const uint8_t command, uint8_t *data, size_t size)
// {
//     switch (command) {
//         case MIPI_DCS_GET_COMPRESSION_MODE:
//         case MIPI_DCS_GET_DISPLAY_ID:
//         case MIPI_DCS_GET_RED_CHANNEL:
//         case MIPI_DCS_GET_GREEN_CHANNEL:
//         case MIPI_DCS_GET_BLUE_CHANNEL:
//         case MIPI_DCS_GET_DISPLAY_STATUS:
//         case MIPI_DCS_GET_POWER_MODE:
//         case MIPI_DCS_GET_ADDRESS_MODE:
//         case MIPI_DCS_GET_PIXEL_FORMAT:
//         case MIPI_DCS_GET_DISPLAY_MODE:
//         case MIPI_DCS_GET_SIGNAL_MODE:
//         case MIPI_DCS_GET_DIAGNOSTIC_RESULT:
//         case MIPI_DCS_GET_SCANLINE:
//         case MIPI_DCS_GET_DISPLAY_BRIGHTNESS:
//         case MIPI_DCS_GET_CONTROL_DISPLAY:
//         case MIPI_DCS_GET_POWER_SAVE:
//         case MIPI_DCS_READ_DDB_START:
//         case MIPI_DCS_READ_DDB_CONTINUE:
//             mipi_display_write_command(command);
//             mipi_display_read_data(data, size);
//             break;
//         default:
//             mipi_display_write_command(command);
//             mipi_display_write_data(data, size);
//     }
// }

void
mipi_display_close()
{

}