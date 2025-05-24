#ifndef _TIMER_H
#define _TIMER_H

#include <types.h>
#include <layout.h>


#define SP804_REG_LOAD      ((u32*)(SP804_BASE + 0x00))
#define SP804_REG_VAL       ((u32*)(SP804_BASE + 0x04))
#define SP804_REG_CTRL      ((u32*)(SP804_BASE + 0x08))
#define SP804_REG_IRQCNTL   ((u32*)(SP804_BASE + 0x0c))
#define SP804_REG_RAWIRQ    ((u32*)(SP804_BASE + 0x10))
#define SP804_REG_MSKIRQ    ((u32*)(SP804_BASE + 0x14))
#define SP804_REG_RELOAD    ((u32*)(SP804_BASE + 0x18))
#define SP804_REG_PREDIV    ((u32*)(SP804_BASE + 0x1c))
#define SP804_REG_FREECNT   ((u32*)(SP804_BASE + 0x20))


#define SP804_ENABLE        (1 << 7)
#define SP804_INT_ENABLE    (1 << 5)
#define SP804_23_BIT        (1 << 1)
#define SP804_PRESCALE_256  (2 << 2)


#define TIMER_CS            ((u32*)(TIMER_BASE + 0x00))
#define TIMER_CLK_LO        ((u32*)(TIMER_BASE + 0x04))
#define TIMER_CLK_HI        ((u32*)(TIMER_BASE + 0x08))
#define TIMER_CMP0          ((u32*)(TIMER_BASE + 0x0c))
#define TIMER_CMP1          ((u32*)(TIMER_BASE + 0x10))
#define TIMER_CMP2          ((u32*)(TIMER_BASE + 0x14))
#define TIMER_CMP3          ((u32*)(TIMER_BASE + 0x18))


#define LOCAL_TIMER_CTRL    ((u32*)(CTRL_BASE + 0x34))
#define LOCAL_TIMER_IRQ     ((u32*)(CTRL_BASE + 0x38))
#define TIMER_CTRL0         ((u32*)(CTRL_BASE + 0x40))
#define TIMER_CTRL1         ((u32*)(CTRL_BASE + 0x44))
#define TIMER_CTRL2         ((u32*)(CTRL_BASE + 0x48))
#define TIMER_CTRL3         ((u32*)(CTRL_BASE + 0x4c))
#define IRQ_SRC0            ((u32*)(CTRL_BASE + 0x60))
#define IRQ_SRC1            ((u32*)(CTRL_BASE + 0x64))
#define IRQ_SRC2            ((u32*)(CTRL_BASE + 0x68))
#define IRQ_SRC3            ((u32*)(CTRL_BASE + 0x6c))


void sleep_cycles(u64 n_cycles);
void sleep_qs(u64 qs);

void timer_init(u32 freq);

#endif
