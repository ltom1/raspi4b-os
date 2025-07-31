#include <types.h>
#include <dbg.h>
#include <std.h>
#include <fs/vfs.h>
#include <hw/mbox.h>
#include <hw/dma.h>
#include <mbox.h>
#include <prop.h>
#include <vmem.h>
#include <asm.h>
#include <timer.h>
#include <pmem.h>
#include <uart.h>
#include <irq.h>
#include <gic.h>
#include <fb.h>
#include <emmc.h>
#include <hw/timer.h>
#include <hw/gic.h>
#include <fs/mbr.h>
#include <fs/fat32.h>
#include <alloc.h>
#include <buddy.h>
#include <bump.h>
#include <proc.h>
#include <drive.h>
#include <elf64.h>
#include <smp.h>
#include <sched.h>
#include <hw/emmc.h>


buddy_alloc_t heap;
fat32_t *fs;
emmc_device_t sd;
mbr_t mbr; 


void kmain(void) {
    dbg_init();
    dbg_info("Hello world!\n");
    dbg_info("fb: %x\n\n", fb);

    pmem_init();
    
    irq_init();

    // enable timer register access for user processes
    asm_set_cntkctl(1);
    // timer_init(1, 3000, CORE0);

    dbg_info("EMMC: %x\n", prop_get_clock_rate(CLOCK_EMMC));
    dbg_info("UART: %x\n", prop_get_clock_rate(CLOCK_UART));
    dbg_info("ARM: %x\n", prop_get_clock_rate(CLOCK_ARM));
    dbg_info("CORE: %x\n", prop_get_clock_rate(CLOCK_CORE));

    heap = CREATE_BUDDY_ALLOCATOR(16);
    heap.alloc.init((void*)&heap);

    dbg_info("REV: %x\n", prop_get_board_rev());
    dbg_info("MEM: %x\n", prop_get_mem(GET_ARM_MEM));
    dbg_info("GPU: %x\n", prop_get_mem(GET_VC_MEM));

    emmc_init();
    emmc_read_blocks(&mbr, 0, 1);

    fs = fat32_init((alloc_t*)&heap, (drive_t*)&sd, &mbr.part[1]);

    file_t *init_elf = fat32_load_file(fs, (alloc_t*)&heap, "/PROG/INIT.ELF");
    pcb_t *init = proc_create(0, "INIT.ELF", 8, init_elf);

    init->prio = 1;

    dbg_info("CORE %u\n", asm_get_core());
    smp_init();

    asm_irq_enable();
    while(1) schedule();
}
