/*

MIT License

Copyright (c) 2021-2022 Mika Tuupola

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

#ifndef _HAGL_HAL_H
#define _HAGL_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <hagl/backend.h>

#include "hagl_hal_color.h"

#define hagl_hal_debug(fmt, ...) \
    do { if (HAGL_HAL_DEBUG) printf("[HAGL HAL] " fmt, __VA_ARGS__); } while (0)

/* Default config is ok for Pimoroni Pico Display Pack. When compiling */
/* you can override these by including an user config header file first. */
/* https://shop.pimoroni.com/products/pico-display-pack */

// #ifndef MIPI_DISPLAY_PIN_CS
// #define MIPI_DISPLAY_PIN_CS         (17)
// #endif
// #ifndef MIPI_DISPLAY_PIN_DC
// #define MIPI_DISPLAY_PIN_DC         (16)
// #endif
// #ifndef MIPI_DISPLAY_PIN_RST
// #define MIPI_DISPLAY_PIN_RST        (-1)
// #endif
// #ifndef MIPI_DISPLAY_PIN_BL
// #define MIPI_DISPLAY_PIN_BL         (20)
// #endif
// #ifndef MIPI_DISPLAY_PIN_CLK
// #define MIPI_DISPLAY_PIN_CLK        (18)
// #endif
// #ifndef MIPI_DISPLAY_PIN_MOSI
// #define MIPI_DISPLAY_PIN_MOSI       (19)
// #endif
// #ifndef MIPI_DISPLAY_PIN_MISO
// #define MIPI_DISPLAY_PIN_MISO       (-1)
// #endif
#ifndef MIPI_DISPLAY_PIXEL_FORMAT
#define MIPI_DISPLAY_PIXEL_FORMAT   (MIPI_DCS_PIXEL_FORMAT_16BIT)
#endif
#ifndef MIPI_DISPLAY_ADDRESS_MODE
#define MIPI_DISPLAY_ADDRESS_MODE   (MIPI_DCS_ADDRESS_MODE_BGR)
#endif

/* Default to invert because Pimoroni Pico Display Pack needs it. */
#ifndef MIPI_DISPLAY_INVERT
#define MIPI_DISPLAY_INVERT
#elif MIPI_DISPLAY_INVERT == 0
#undef MIPI_DISPLAY_INVERT
#endif

#ifndef MIPI_DISPLAY_WIDTH
#define MIPI_DISPLAY_WIDTH          (320)
#endif
#ifndef MIPI_DISPLAY_HEIGHT
#define MIPI_DISPLAY_HEIGHT         (240)
#endif
#ifndef MIPI_DISPLAY_DEPTH
#define MIPI_DISPLAY_DEPTH          (16)
#endif
#ifndef MIPI_DISPLAY_OFFSET_X
#define MIPI_DISPLAY_OFFSET_X       (80)
#endif
#ifndef MIPI_DISPLAY_OFFSET_Y
#define MIPI_DISPLAY_OFFSET_Y       (40)
#endif

#ifndef HAGL_HAL_PIXEL_SIZE
#define HAGL_HAL_PIXEL_SIZE         (1)
#endif

/* These are used internally. */
#define HAGL_PICO_MIPI_DISPLAY_WIDTH      (MIPI_DISPLAY_WIDTH / HAGL_HAL_PIXEL_SIZE)
#define HAGL_PICO_MIPI_DISPLAY_HEIGHT     (MIPI_DISPLAY_HEIGHT / HAGL_HAL_PIXEL_SIZE)
#define HAGL_PICO_MIPI_DISPLAY_DEPTH      (MIPI_DISPLAY_DEPTH)

#ifdef HAGL_HAL_USE_TRIPLE_BUFFER
#define HAGL_HAS_HAL_BACK_BUFFER
#endif

#ifdef HAGL_HAL_USE_DOUBLE_BUFFER
#define HAGL_HAS_HAL_BACK_BUFFER
#endif

#ifdef HAGL_HAL_USE_SINGLE_BUFFER
#undef HAGL_HAS_HAL_BACK_BUFFER
#endif

/**
 * Initialize the HAL
 */
void hagl_hal_init(hagl_backend_t *backend);

#ifdef __cplusplus
}
#endif
#endif /* _HAGL_HAL_H */