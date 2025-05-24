#include <types.h>
#include <irq.h>
#include <int.h>
#include <dbg.h>
#include <asm.h>
#include <ic.h>


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


void irq_handle_invalid(u64 vec, u64 esr, u64 elr) {
    dbg_info("%s\nESR: %x\nELR: %x\n", entry_labels[vec], esr, elr);
    asm_halt();
}


void irq_handle(void) {
   dbg_info("Handled\n");
}
