#include <hw/gic.h>
#include <proc.h>
#include <types.h>
#include <timer.h>
#include <hw/ic.h>
#include <hw/timer.h>
#include <asm.h>
#include <sched.h>
#include <gic.h>
#include <dbg.h>


void sleep_cycles(u64 n_cycles) {
    while (n_cycles--) 
        ASM("nop" : : : "memory");
}


void sleep_qs(u64 qs) {

    register u64 freq, goal, count;

    freq = asm_get_timer_freq();

    // get current count
    goal = asm_get_time();

    goal += ((freq / 1000) * qs) / 1000; 

    do (count = asm_get_time());
    while (count < goal);
}


u32 interval[4];
u32 cur_val[4];

void timer_init(u8 n, u32 delay_ms, u8 target) {

    gic_enable();
    gic_int_enable(IRQ_SYS_TIMER0 + n, target);

    interval[n] = (delay_ms * CLOCKHZ) / 1000;

    cur_val[n] = SYS_TIMER->lo;
    cur_val[n] += interval[n];
    SYS_TIMER->cmp[n] = cur_val[n];
}


void timer_handle_irq(u8 n) {

    cur_val[n] += interval[n];
    SYS_TIMER->cmp[n] = cur_val[n];
    SYS_TIMER->cs = (1 << n);

    dbg_info("TICK!\n");

    // schedule
    cur_proc[asm_get_core()]->counter--;
    // current process has time slices left
    if (cur_proc[asm_get_core()]->counter > 0 || cur_proc[asm_get_core()]->preempt_count > 0) return;

    cur_proc[asm_get_core()]->counter = 0;
    dbg_info("SCHEDULE\n");

    // mark irq as completed
    GICC->eoir = IRQ_SYS_TIMER0 + n;
    *GICC_DIR = IRQ_SYS_TIMER0 + n;
    schedule();
}
