#pragma once

#include <types.h>
#include <hw/mmap.h>


#define CLOCKHZ     1000000


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


#define SP804           ((sp804_t*)SP804_BASE)

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


#define SYS_TIMER       ((sys_timer_t*)SYS_TIMER_BASE)


typedef struct PACKED {
    u32 ctrl;
    u32 _res0[2];
    u32 core_irq_ctrl;
    u32 pmu_ctrl_set;
    u32 pmu_ctrl_clr;
    u32 _res1[3];
    u32 peri_irq_route0;
    u32 _res2[2];
    u32 axi_quiet_time;
    u32 local_timer_ctrl;
    u32 local_timer_irq;
    u32 _res3;
    u32 timer_ctrl[4];
    u32 mbox_ctrl[4];
    u32 irq_src[4];
    u32 fiq_src[4];
} arm_local_t;


#define ARM_LOCAL ((arm_local_t*)CTRL_BASE)

#define LOCAL_TIMER_IRQ         53
#define LOCAL_TIMER_EN          (1 << 28)
#define LOCAL_TIMER_IRQ_EN      (1 << 29)
#define LOCAL_TIMER_IRQ_RELOAD  (1 << 30)
#define LOCAL_TIMER_IRQ_CLR     (1 << 31)
