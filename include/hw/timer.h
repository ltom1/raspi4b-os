#pragma once

#include <types.h>
#include <hw/mmap.h>


typedef struct PACKED {
    u32 load;
    u32 val;
    u32 ctrl;
    u32 irqctrl;
    u32 rawirq;
    u32 mskirq;
    u32 reload;
    u32 prediv;
    u32 freecnt;
} sp804_t;


#define SP804       ((sp804_t*)SP804_BASE)

#define SP804_ENABLE        (1 << 7)
#define SP804_INT_ENABLE    (1 << 5)
#define SP804_23_BIT        (1 << 1)
#define SP804_PRESCALE_256  (2 << 2)


typedef struct PACKED {
    u32 cs;
    u32 lo;
    u32 hi;
    u32 cmp[4];
} sys_timer_t;


#define SYS_TIMER   ((sys_timer_t*)SYS_TIMER_BASE)


typedef struct PACKED {
    u32 ctrl;
    u32 irq_clr;
    u32 _res0;
    u32 timer_ctrl[4];
    u32 mbox_ctrl[4];
    u32 irq_src[4];
    u32 fiq_src[4];
} local_timer_t;
