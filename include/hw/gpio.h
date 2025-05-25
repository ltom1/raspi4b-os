#pragma once

#include <types.h>
#include <hw/mmap.h>


typedef struct PACKED {
    u32 fsel[6];
    u32 _res0;
    u32 set[2];
    u32 _res1;
    u32 clr[2];
    u32 _res2;
    u32 lev[2];
    u32 _res3;
    u32 eds[2];
    u32 _res4;
    u32 ren[2];
    u32 _res5;
    u32 fen[2];
    u32 _res6;
    u32 hen[2];
} gpio_t;


#define GPIO        ((gpio_t*)GPIO_BASE)
