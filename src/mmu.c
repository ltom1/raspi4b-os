#include <types.h>
#include <layout.h>
#include <asm.h>
#include <mmu.h>


void mmu_mair_init(void) {

    u64 r = (MAIR_MEM << 0) |
            (MAIR_DEV << 8);

    ASM("msr mair_el1, %0" : : "r" (r));
}


void mmu_tcr_init(void) {

    u64 r = (16ul <<    0) |    // T0SZ -> 4 lvl pt
            (0b01ul <<  8) |    // IRGN0 -> write back
            (0b01ul << 10) |    // ORGN0 -> write back
            (0b11ul << 12) |    // SH0 -> inner
            (0b00ul << 14) |    // TG0 -> 4kb
            (16ul <<   16) |    // T1SZ -> 4 lvl pt
            (0b01ul << 24) |    // IRGN1 -> write back
            (0b01ul << 26) |    // ORGN1 -> write back
            (0b11ul << 28) |    // SH1 -> inner
            (0b00ul << 30) |    // TG1 -> 4kb
            (0b101ul << 32)|    // IPS -> 32 bit physical addresses !ATTENTION!
            (0b00ul << 37);     // TBI -> no tagging

    ASM("msr tcr_el1, %0; isb" : : "r" (r));
}


void mmu_change_pt(u64 ttbr0_paddr, u64 ttbr1_paddr) {

    if (ttbr0_paddr) ASM("msr ttbr0_el1, %0" : : "r" (ttbr0_paddr));
    if (ttbr1_paddr) ASM("msr ttbr1_el1, %0" : : "r" (ttbr1_paddr));
}


void mmu_enable(void) {

    u64 r;
    ASM("dsb ish; isb; mrs %0, sctlr_el1" : "=r" (r));

    r |= 1; // M bit

    ASM("msr sctlr_el1, %0; isb" : : "r" (r));
}
