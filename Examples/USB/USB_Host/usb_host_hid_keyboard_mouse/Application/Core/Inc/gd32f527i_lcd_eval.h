/*!
    \file    gd32f527i_lcd_eval.h
    \brief   the header file of LCD

    \version 2026-01-31, V1.4.0, firmware for GD32F527
*/

/*
    Copyright (c) 2026, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32F527I_LCD_EVAL_H
#define GD32F527I_LCD_EVAL_H

#include "stdint.h"
#include "lcd_font.h"

/* definitions of color */
#define LCD_COLOR_WHITE          0xFFFFU
#define LCD_COLOR_BLACK          0x0000U
#define LCD_COLOR_GREY           0xF7DEU
#define LCD_COLOR_BLUE           0x001FU
#define LCD_COLOR_BLUE2          0x051FU
#define LCD_COLOR_RED            0xF800U
#define LCD_COLOR_MAGENTA        0xF81FU
#define LCD_COLOR_GREEN          0x07E0U
#define LCD_COLOR_CYAN           0x7FFFU
#define LCD_COLOR_YELLOW         0xFFE0U

/* definitions of lines depending on the chosen fonts */
#define LCD_LINE_0               LINE(0U)
#define LCD_LINE_1               LINE(1U)
#define LCD_LINE_2               LINE(2U)
#define LCD_LINE_3               LINE(3U)
#define LCD_LINE_4               LINE(4U)
#define LCD_LINE_5               LINE(5U)
#define LCD_LINE_6               LINE(6U)
#define LCD_LINE_7               LINE(7U)
#define LCD_LINE_8               LINE(8U)
#define LCD_LINE_9               LINE(9U)
#define LCD_LINE_10              LINE(10U)
#define LCD_LINE_11              LINE(11U)
#define LCD_LINE_12              LINE(12U)
#define LCD_LINE_13              LINE(13U)
#define LCD_LINE_14              LINE(14U)
#define LCD_LINE_15              LINE(15U)
#define LCD_LINE_16              LINE(16U)
#define LCD_LINE_17              LINE(17U)
#define LCD_LINE_18              LINE(18U)
#define LCD_LINE_19              LINE(19U)
#define LCD_LINE_20              LINE(20U)
#define LCD_LINE_21              LINE(21U)
#define LCD_LINE_22              LINE(22U)
#define LCD_LINE_23              LINE(23U)
#define LCD_LINE_24              LINE(24U)
#define LCD_LINE_25              LINE(25U)
#define LCD_LINE_26              LINE(26U)
#define LCD_LINE_27              LINE(27U)
#define LCD_LINE_28              LINE(28U)
#define LCD_LINE_29              LINE(29U)
#define LCD_LINE_30              LINE(30U)
#define LCD_LINE_31              LINE(31U)
#define LCD_LINE_32              LINE(32U)
#define LCD_LINE_33              LINE(33U)
#define LCD_LINE_34              LINE(34U)
#define LCD_LINE_35              LINE(35U)
#define LCD_LINE_36              LINE(36U)
#define LCD_LINE_37              LINE(37U)
#define LCD_LINE_38              LINE(38U)
#define LCD_LINE_39              LINE(39U)
#define LCD_LINE_40              LINE(40U)
#define LCD_LINE_41              LINE(41U)
#define LCD_LINE_42              LINE(42U)
#define LCD_LINE_43              LINE(43U)
#define LCD_LINE_44              LINE(44U)
#define LCD_LINE_45              LINE(45U)
#define LCD_LINE_46              LINE(46U)
#define LCD_LINE_47              LINE(47U)
#define LCD_LINE_48              LINE(48U)
#define LCD_LINE_49              LINE(49U)
#define LCD_LINE_50              LINE(50U)
#define LCD_LINE_51              LINE(51U)
#define LCD_LINE_52              LINE(52U)
#define LCD_LINE_53              LINE(53U)
#define LCD_LINE_54              LINE(54U)
#define LCD_LINE_55              LINE(55U)
#define LCD_LINE_56              LINE(56U)
#define LCD_LINE_57              LINE(57U)
#define LCD_LINE_58              LINE(58U)
#define LCD_LINE_59              LINE(59U)

#define LCD_LAYER_BACKGROUND     0x0000U
#define LCD_LAYER_FOREGROUND     0x0001U

#define LCD_LINEDIR_HORIZONTAL   0x0000U
#define LCD_LINEDIR_VERTICAL     0x0001U

#define LCD_DEFAULT_FONT         font16x24

#define LCD_PIXEL_WIDTH          ((uint16_t)480U)
#define LCD_PIXEL_HEIGHT         ((uint16_t)272U)

#define LCD_SCREEN_HORIZONTAL    1U    /* set it to 0 and VERTICALSCREEN to 1 can change screen into vertical */
#define LCD_SCREEN_VERTICAL      0U

typedef struct {
    int16_t x;
    int16_t y;
} point_struct, *ptr_point;

/* function declarations */
/* initializes the LCD of GD EVAL board */
void gd_eval_lcd_init(void);
/* initialize the LCD GPIO and TLI */
void lcd_init(void);
/* initialize TLI layer0 or layer1 */
void lcd_layer_init(uint32_t layer, uint32_t width, uint32_t height);
/* enable the LCD layer0 or layer1 */
void lcd_layer_enable(uint32_t layer);
/* set the LCD layer */
void lcd_layer_set(uint32_t layer);
/* set the transparency of LCD */
void lcd_transparency_set(uint8_t trans);
/* configure the packeted pixel format */
void lcd_pixel_format_config(uint32_t pixel_format);
/* configure the frame buffer base address */
void lcd_address_config(uint32_t address);

/* clear the LCD with specified color */
void lcd_clear(uint16_t color);
/* set the text font */
void lcd_font_set(font_struct *font);
/* get the text font */
font_struct *lcd_font_get(void);
/* set the text color */
void lcd_text_color_set(uint16_t color);
/* get the current text color */
uint16_t lcd_text_color_get(void);
/* set the background color */
void lcd_background_color_set(uint16_t color);
/* get the current background color */
uint16_t lcd_background_color_get(void);

/* set point with the specified position and color */
void lcd_point_set(uint16_t xpos, uint16_t ypos, uint16_t color);
/* get the color of point with the specified position */
uint16_t lcd_point_get(uint16_t xpos, uint16_t ypos);
/* draw a line on LCD */
void lcd_line_draw(uint16_t xpos, uint16_t ypos, uint16_t length, uint8_t line_direction);
/* draw a rectangle on LCD */
void lcd_rectangle_draw(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height);
/* draw a circle on LCD using Bresenham algorithm */
void lcd_circle_draw(uint16_t xpos, uint16_t ypos, uint16_t radius);
/* draw a ellipse on LCD using the midpoint ellipse algorithm */
void lcd_ellipse_draw(uint16_t xpos, uint16_t ypos, uint16_t axis1, uint16_t axis2);
/* fill the whole rectangle */
void lcd_rectangle_fill(uint16_t xpos, uint16_t ypos, uint16_t width, uint16_t height);
/* display the character on LCD */
void lcd_char_display(uint16_t line, uint16_t column, uint8_t ascii);
/* display the vertical character on LCD */
void lcd_vertical_char_display(uint16_t line, uint16_t column, uint8_t ascii);
/* display the string on LCD */
void lcd_string_display(uint16_t stringline, uint8_t *ptr);
/* display the vertical string on LCD */
void lcd_vertical_string_display(uint16_t stringline, uint16_t offset, uint8_t *ptr);

#endif /* GD32F527I_LCD_EVAL_H */
