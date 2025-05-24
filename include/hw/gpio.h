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

#define GPFSEL0     ((u32*)(GPIO_BASE + 0x00))
#define GPFSEL1     ((u32*)(GPIO_BASE + 0x04))
#define GPFSEL2     ((u32*)(GPIO_BASE + 0x08))
#define GPFSEL3     ((u32*)(GPIO_BASE + 0x0c))
#define GPFSEL4     ((u32*)(GPIO_BASE + 0x10))
#define GPFSEL5     ((u32*)(GPIO_BASE + 0x14))

#define GPSET0      ((u32*)(GPIO_BASE + 0x1c))
#define GPSET1      ((u32*)(GPIO_BASE + 0x20))

#define GPCLR0      ((u32*)(GPIO_BASE + 0x28))
#define GPCLR1      ((u32*)(GPIO_BASE + 0x2c))
    
#define GPLEV0      ((u32*)(GPIO_BASE + 0x34))
#define GPLEV1      ((u32*)(GPIO_BASE + 0x38)) 

#define GPEDS0      ((u32*)(GPIO_BASE + 0x40)) 
#define GPEDS1      ((u32*)(GPIO_BASE + 0x44)) 

#define GPREN0      ((u32*)(GPIO_BASE + 0x4c)) 
#define GPREN1      ((u32*)(GPIO_BASE + 0x50)) 
    
#define GPFEN0      ((u32*)(GPIO_BASE + 0x58))
#define GPFEN1      ((u32*)(GPIO_BASE + 0x5c))

#define GPHEN0      ((u32*)(GPIO_BASE + 0x64))
#define GPHEN1      ((u32*)(GPIO_BASE + 0x68))

#define GPLEN0      ((u32*)(GPIO_BASE + 0x70))
#define GPLEN1      ((u32*)(GPIO_BASE + 0x74))

#define GPAREN0     ((u32*)(GPIO_BASE + 0x7c))
#define GPAREN1     ((u32*)(GPIO_BASE + 0x80))

#define GPAFEN0     ((u32*)(GPIO_BASE + 0x88))
#define GPAFEN1     ((u32*)(GPIO_BASE + 0x8c))
