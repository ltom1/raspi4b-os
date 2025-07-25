#include <types.h>
#include <asm.h>
#include <hw/mmap.h>
#include <uart.h>
#include <hw/gpio.h>
#include <hw/aux.h>
#include <timer.h>
#include <gpio.h>


void uart1_init(void) {

    *AUX_ENABLE |= 1;
    // disable UART transmitter/receiver
    AUX_MU->ctrl = 0; 
    // 8-bit mode
    AUX_MU->lcr = 3;
    AUX_MU->mcr = 0;
    // no transmit/receive interrupts
    AUX_MU->ier = 0;
    // clear FIFO
    AUX_MU->iir = 0x6;
    // baudrate 
    AUX_MU->baud = 541;

    // enable gpio pins 14 and 15
    // alternate function 5 (transmit/receive uart1)
    gpio_fsel(14, ALT_5);
    gpio_fsel(15, ALT_5);

    // enable UART transmitter/receiver
    AUX_MU->ctrl = 3; 
}


void uart1_putc(char c) {
    
    // wait until the FIFO accepts data
    do ASM("nop" : : : "memory");
    while (!(AUX_MU->lsr & (1 << 5)));

    AUX_MU->io = c;
}


void uart1_puts(const char *str) {

    while (*str) {        
        // newline needs a carriage return
        if (*str == '\n')
            uart1_putc('\r');
        uart1_putc(*str++);
    }
}

    
char uart1_getc(void) {
    
    // wait until data is available
    do ASM("nop" : : : "memory");
    while (!(AUX_MU->lsr & 1));

    char c = (char)AUX_MU->io;

    // convert carriage return into newline
    return c == 'r' ? '\n' : c;
}
