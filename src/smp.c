#include <types.h>
#include <smp.h>
#include <sched.h>
#include <asm.h>
#include <pmem.h>
#include <proc.h>
#include <dbg.h>
#include <irq.h>
#include <hw/mmap.h>


u64 kstacks[N_CORES];
u64 *spin_core = (u64*)SPIN_CORE_BASE;


void smp_init(void) {

    kstacks[0] = (u64)_start;

    for (u64 i = 1; i < 3; i++) {
        kstacks[i] = pmem_alloc(STACK_SIZE) + STACK_SIZE;
        spin_core[i] = V2P(smp_core_stub);
    }

    ASM("dsb sy; sev");
}


void smp_core_init(u8 core) {

    dbg_info("Hello from core %u\n", core);
    dbg_info("Bye from core %u\n", core);

    dbg_info("CORE %u\n", asm_get_core());

    irq_init();

    // allow userspace to access the timer registers
    asm_set_cntkctl(1);

    asm_irq_enable();
    while (1) schedule(); 

    while(1);
}
