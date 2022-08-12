/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author        Notes
 * 2021-10-18     Meco Man      First version
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <rtconfig.h>
#include "lvgl_support.h"

#define LV_USE_PERF_MONITOR     1
#define LV_COLOR_DEPTH          16
#define LV_HOR_RES_MAX          LCD_WIDTH
#define LV_VER_RES_MAX          LCD_HEIGHT

#define LV_USE_NXP_SOC          1
//#define LV_USE_GPU_NXP_PXP        1
//#define LV_USE_GPU_NXP_VG_LITE    0
//#define LV_USE_GPU_NXP_PXP_AUTO_INIT 1

#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 64

#define LV_FONT_UNSCII_8			1
#define LV_FONT_UNSCII_16			1
#define LV_FONT_MONTSERRAT_12       1
#define LV_FONT_MONTSERRAT_16       1
#define LV_FONT_MONTSERRAT_40       1
#define LV_COLOR_SCREEN_TRANSP      1

#endif
