#include <types.h>
#include <dbg.h>
#include <mbox.h>
#include <prop.h>
#include <vmem.h>
#include <asm.h>
#include <timer.h>
#include <pmem.h>
#include <uart.h>
#include <irq.h>
#include <gic.h>


void kmain(void) {

    dbg_init();
    dbg_info("Hello world!\n");
    pmem_init();
    irq_init();
    asm_irq_enable();
    timer_init(0x4000);


    u64 x1 = pmem_alloc(10);
    u64 x2 = pmem_alloc(20);
    pmem_free(x1, 10);
    u64 x3 = pmem_alloc(5);

    u64 serial = prop_get_serial();   
    dbg_info("serial: %x\n", serial);

    sleep_qs(2000000);
    u64 mac;
    prop_get_mac((u8*)&mac);   
    dbg_info("mac: %x\n", mac);

    sleep_qs(2000000);
    u64 mem = prop_get_arm_mem();
    dbg_info("mem: %x\n", mem);

    while(1) {
        uart1_putc(uart1_getc());
    }
}
