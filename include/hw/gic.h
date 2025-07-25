#pragma once

#include <types.h>
#include <hw/mmap.h>


typedef struct PACKED {
    u32 ctrl;
    u32 type;
    u32 iid;

    u32 _res0[29];

    u32 group[15];

    u32 _res1[17];

    u32 int_set_enable[32];
    u32 int_clr_enable[32];

    u32 int_set_pend[32];
    u32 int_clr_pend[32];
    u32 int_set_active[32];
    u32 int_clr_active[32];
    u32 int_prio[32];

    u32 _res2[224];
    u32 target[128];
} gicd_t;

typedef struct PACKED {
    u32 ctrl;
    u32 prio_mask;

    u32 bpr;
    u32 iar;
    u32 eoir;
    u32 rpr;
    u32 hppir;

    u32 abpr;
    u32 aiar;
    u32 aeoir;
    u32 ahppir;
} gicc_t;

#define GICC_DIR        ((u32*)GICC_BASE + 0x1000)

typedef enum {
    IRQ_SPURIOUS    = 1023,
    IRQ_SYS_TIMER0  = 96,
    IRQ_SYS_TIMER1  = 97,
    IRQ_SYS_TIMER2  = 98,
    IRQ_SYS_TIMER3  = 99,
} irq_id_t;


typedef enum {
    CORE0 = 1,
    CORE1 = 2,
    CORE2 = 4,
    CORE3 = 8
} core_t;


#define GICD            ((gicd_t*)GICD_BASE)
#define GICC            ((gicc_t*)GICC_BASE)
