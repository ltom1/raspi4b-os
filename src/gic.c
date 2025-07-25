#include <types.h>
#include <hw/gic.h>
#include <dbg.h>


void gic_int_enable(u64 n, u8 target) {

    u32 r;

    // enable interrupt
    r = GICD->int_set_enable[n / 32];
    r |= (1 << (n % 32));
    GICD->int_set_enable[n / 32] = r;

    // set group to 1
    r = GICD->group[n / 32];
    r |= (1 << (n % 32));
    GICD->group[n / 32] = r;

    // set target cores
     r = GICD->target[n / 4];
     r &= ~(0xff << (8 * (n % 4)));  // clear
     r |= target << (8 * (n % 4));   // set new val
     GICD->target[n / 4] = r;
}


void gic_int_disable(u64 n) {

    u32 r;

    // disable interrupt
    r = GICD->int_set_enable[n / 32];
    r &= ~(1 << (n % 32));
    GICD->int_set_enable[n / 32] = r;

    // clear target
    r = GICD->target[n / 4];
    r &= ~(0xff << (8 * (n % 4)));
    GICD->target[n / 4] = r;
}


void gic_enable(void) {

    // enable group0 and group1
    GICD->ctrl = 0b11;
    GICC->ctrl = 0b11;

    dbg_info("GICD->ctrl = %x\n", GICD->ctrl);
    dbg_info("GICC->ctrl = %x\n", GICC->ctrl);
}


void gic_disable(void) {

    // disable group0 and group1
    GICD->ctrl = 0b00;
    GICC->ctrl = 0b00;
}
