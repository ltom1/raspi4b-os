#include <types.h>
#include <gic.h>


void gic_int_enable(u64 n) {

    u32 r;

    // enable interrupt
    r = GICD->int_set_enable[n / 32];
    r |= (1 << (n % 32));
    GICD->int_set_enable[n / 32] = r;

    // set group to 0
    r = GICD->group[n / 32];
    r &= ~(1 << (n % 32));
    GICD->int_set_enable[n / 32] = r;
}


void gic_int_disable(u64 n) {

    u32 r;

    // disable interrupt
    r = GICD->int_set_enable[n / 32];
    r &= ~(1 << (n % 32));
    GICD->int_set_enable[n / 32] = r;
}


void gic_enable(void) {

    // enable group0 and group1
    GICD->ctrl = 0b11;
    GICC->ctrl = 0b11;
}


void gic_disable(void) {

    // disable group0 and group1
    GICD->ctrl = 0b00;
    GICC->ctrl = 0b00;
}
