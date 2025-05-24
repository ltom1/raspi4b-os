#include <types.h>
#include <asm.h>
#include <layout.h>
#include <uart.h>
#include <gpio.h>
#include <aux.h>
#include <timer.h>


void uart1_init(void) {

    *AUX_ENABLE |= 1;
    // disable UART transmitter/receiver
    *AUX_MU_CTRL = 0; 
    // 8-bit mode
    *AUX_MU_LCR = 3;
    *AUX_MU_MCR = 0;
    // no transmit/receive interrupts
    *AUX_MU_IER = 0;
    // clear FIFO
    *AUX_MU_IIR = 0x6;
    // baudrate 
    *AUX_MU_BAUD = 541;

    // enable gpio pins 14 and 15
    // alternate function 5 (transmit/receive uart1)
    register u32 r = *GPFSEL1;
    // clear bits for gpio pins 14 and 15
    r &= ~((7 << 12) | (7 << 15));
    // set second bit for each pin -> alt fun 5
    r |= (2 << 12) | (2 << 15);
    *GPFSEL1 = r;

    // enable UART transmitter/receiver
    *AUX_MU_CTRL = 3; 
}


void uart1_putc(char c) {
    
    // wait until the FIFO accepts data
    do ASM("nop" : : : "memory");
    while (!(*AUX_MU_LSR & (1 << 5)));

    *AUX_MU_IO = c;
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
    while (!(*AUX_MU_LSR & 1));

    char c = (char)*AUX_MU_IO;

    // convert carriage return into newline
    return c == 'r' ? '\n' : c;
}
