#include <types.h>
#include <uart.h>


void kmain(void) {

    uart1_init();
    uart1_puts("Hello world!\nIt works!");

    while(1);
}
