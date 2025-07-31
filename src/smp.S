.equ KERNEL_VMEM_BASE, 0xffff000000000000

.global smp_core_stub
smp_core_stub:
    // read CPU info
    mrs     x4, mpidr_el1
    and     x4, x4, #3

    // remove kernel base because we are still using physical memory
    ldr     x3, =KERNEL_VMEM_BASE

    // stack right in front of the kernel
    ldr     x2, =kstacks
    sub     x2, x2, x3
    ldr     x1, [x2, x4, lsl #3]

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
    mov     x0, x4
    ldr     x1, =smp_core_init
    br      x1

halt:
    // should not return but halt just in case
    wfe
    b       halt
