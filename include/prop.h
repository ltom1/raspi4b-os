#pragma once

#include <types.h>


typedef enum {
    CLOCK_EMMC  = 1, 
    CLOCK_UART  = 2, 
    CLOCK_ARM   = 3, 
    CLOCK_CORE  = 4, 
} clock_t;


u64 prop_get_serial(void);
bool prop_get_mac(void *buf);
u64 prop_get_mem(u32 tag);
u32 prop_get_clock_rate(u32);
u32 prop_get_board_rev(void);
