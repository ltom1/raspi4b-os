#include <types.h>
#include <hw/gpio.h>


void gpio_fsel(u8 pin, gpio_func_t func) {

    register u32 r = GPIO->fsel[pin / 10]; 

    // clear the corresponding bits in the fsel registers
    r &= ~(0b111 << ((pin % 10) * 3));

    // fill them with the new func
    r |= func << ((pin % 10) * 3);

    GPIO->fsel[pin / 10] = r;
}
