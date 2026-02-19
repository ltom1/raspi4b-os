#include <types.h>
#include <irq.h>
#include <int.h>
#include <log.h>
#include <dbg.h>
#include <timer.h>
#include <proc.h>
#include <asm.h>
#include <hw/ic.h>
#include <hw/gic.h>
#include <hw/timer.h>


const char *entry_labels[] = {
	"SYNC_EL1T",
	"IRQ_EL1T",		
	"FIQ_EL1T",		
	"ERROR_EL1T",		

	"SYNC_EL1H",		
	"IRQ_EL1H",		
	"FIQ_EL1H",		
	"ERROR_EL1H",		

	"SYNC_EL0_64",		
	"IRQ_EL0_64",		
	"FIQ_EL0_64",		
	"ERROR_EL0_64",	

	"SYNC_EL0_32",		
	"IRQ_EL0_32",		
	"FIQ_EL0_32",		
	"ERROR_EL0_32"	
};


void irq_handle_invalid(u64 vec, u64 esr, u64 elr, u64 far) {

//    log_print();
    asm_irq_disable();
    while(1);
    dbg_info(
        "\n%s on CORE %u\nESR: %x\nELR: %x\nFAR: %x\nSP: %x\n\n", 
        entry_labels[vec], asm_get_core(), esr, elr, far, asm_get_sp()
    );

/*
    u64 x = 0;
    for (int i = 0; i < 32; i+=2) {
        x = GICD->int_set_pend[i] + ((u64)GICD->int_set_pend[i+1] << 32);
        dbg_info("%x\n", x);
    }
    */
    asm_irq_disable();
    asm_halt();
}


void irq_handle(void) {
    u32 irq = GICC->iar;

    dbg_info("IRQ: %u\n", irq);

    switch (irq) {
        case IRQ_SYS_TIMER0:
            timer_handle_irq(0);
            break;
        case IRQ_SYS_TIMER1:
            timer_handle_irq(1);
            break;
        case IRQ_SYS_TIMER2:
            timer_handle_irq(2);
            break;
        case IRQ_SYS_TIMER3:
            timer_handle_irq(3);
            break;
        default:
            dbg_info("Unhandled IRQ!\n");
            asm_irq_disable();
            asm_halt();
    }


    GICC->eoir = irq;
    *GICC_DIR = irq;
}


void irq_handle_sync(u64 esr, u64 elr, u64 far) {

    asm_irq_disable();

    if ((esr & ESR_PAGE_FAULT) == ESR_PAGE_FAULT) {
        dbg_info("PAGEFAULT at %x sp %x\n", far, cur_proc[asm_get_core()]->tf->sp);
        vmem_handle_page_fault(far);
    } else {
        irq_handle_invalid(VEC_SYNC_EL0_64, esr, elr, far);
    }

    asm_irq_enable();
}
