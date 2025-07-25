#pragma once

#include <types.h>


#define FB_RES_X    1024
#define FB_RES_Y    768

#define DRAW_PXL(x, y, color)   (fb[(y) * fb_width + (x)] = (color))

extern u32 *fb;
extern u32 fb_width, fb_height, fb_pitch, tty_fg, tty_bg;


void fb_init(u32 width, u32 height);
void fb_draw_rect(u32 x, u32 y, u32 width, u32 height, u32 color);
void fb_draw_circ(u32 x, u32 y, u32 radius, u32 color);
void fb_draw_char(u32 x, u32 y, char c, u32 fg, u32 bg);

void fb_putc(char c);
void fb_scroll(void);
