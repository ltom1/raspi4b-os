#include <types.h>
#include <timer.h>
#include <ic.h>
#include <asm.h>
#include <gic.h>


void sleep_cycles(u64 n_cycles) {
    while (n_cycles--) 
        ASM("nop" : : : "memory");
}


void sleep_qs(u64 qs) {

    register u64 freq, goal, count;

    ASM("mrs %0, cntfrq_el0" : "=r"(freq) : : "memory");

    // get current count
    ASM("mrs %0, cntpct_el0" : "=r"(goal) : : "memory");

    goal += ((freq / 1000) * qs) / 1000; 

    do ASM("mrs %0, cntpct_el0" : "=r"(count) : : "memory");
    while (count < goal);
}


void timer_init(u32 freq) {

    gic_enable();
    for (int i = 0; i < 120; i++) gic_int_enable(i);
    *IRQ0_SET_EN_0 = 0xffffffff;
    *IRQ0_SET_EN_1 = 0xffffffff;
    *IRQ0_SET_EN_2 = 0xffffffff;
    


    sleep_qs(1000000);

    *TIMER_CMP0 = *TIMER_CLK_LO + 1000000;
    *TIMER_CMP1 = *TIMER_CLK_LO + 1000000;
    *TIMER_CMP2 = *TIMER_CLK_LO + 1000000;
    *TIMER_CMP3 = *TIMER_CLK_LO + 1000000;


/*
     // frequency = (clk / 256) * freq
    *SP804_REG_LOAD = 1;

    *SP804_REG_CTRL = 
        SP804_ENABLE | 
        SP804_INT_ENABLE | 
        SP804_23_BIT |
        SP804_PRESCALE_256;


    // configure interrupt controller
    *IRQ0_SET_EN_0 = 1; // TIMER_IRQ
    *IRQ0_SET_EN_0 = 2; // TIMER_IRQ
    *IRQ0_SET_EN_0 = 4; // TIMER_IRQ
    *IRQ0_SET_EN_0 = 8; // TIMER_IRQ

    *TIMER_CMP1 = *TIMER_CLK_LO + 1000000;
    *TIMER_CS = 0;
*/
}
