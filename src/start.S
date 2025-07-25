.section ".text.boot"

.global _start

.equ KERNEL_VMEM_BASE, 0xffff000000000000

// starting point (placed at 0x80000)
_start:
    // read CPU info
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    cbz     x1, cpu_master

// halt every other CPU core
halt:
    wfe
    b       halt

// the first CPU core
cpu_master:

    // remove kernel base because we are still using physical memory
    ldr     x3, =KERNEL_VMEM_BASE

    // stack right in front of the kernel
    ldr     x1, =_start
    sub     x1, x1, x3

    // read current execution lvl
    mrs     x0, CurrentEL
    and     x0, x0, #12     // mask

    // are we already in el1?
    cmp     x0, #4
    beq     el1

    // set stack pointer
    msr     sp_el1, x1
    // enable CNTP timer for el1
    mrs     x0, cnthctl_el2
    orr     x0, x0, #3
    msr     cnthctl_el2, x0
    msr     cntvoff_el2, xzr
    // enable aarch64 in el1
    mov     x0, #(1 << 31)      // aarch64
    orr     x0, x0, #(1 << 1)   // data cache cleaning on invalidation
    msr     hcr_el2, x0
    mrs     x0, hcr_el2
    // change execution level to EL1 + mask irq, fiq, serror, debug exception
    mov     x2, #0x3c4
    msr     spsr_el2, x2

    // jump to el1 using an eret
    adr     x2, el1
    msr     elr_el2, x2
    eret

el1:
    msr     spsel, #1
    mov     sp, x1

    // clear BSS section
    ldr     x1, =__bss_start
    sub     x1, x1, x3
    ldr     w2, =__bss_size
    cbz     w2, break

loop:
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, loop

break:
    // set up vmem and paging
    bl      vmem_bootstrap
    bl      mmu_mair_init
    bl      mmu_tcr_init

    ldr     x0, pt0_user
    sub     x0, x0, x3

    ldr     x1, pt0_kernel
    sub     x1, x1, x3

    bl      mmu_change_pt

    bl      mmu_enable

    // change stack pointer to the higher half
    add     sp, sp, x3

    // enable SIMD NEON and FP support
    // disable access trapping in EL1 and EL0.
    mov     x1, #(0x3 << 20) // FPEN disables trapping to EL1.
    msr     cpacr_el1, x1
    isb

    // jump to higher half C code
    ldr     x0, =kmain
    br      x0

    // should not return but halt just in case
    b       halt
