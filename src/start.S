.section ".text.boot"

.global _start

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
    
    // initialize stack
    ldr     x1, =_start
    mov     sp, x1


    // clear BSS section
    ldr     x1, =__bss_start
    ldr     w2, =__bss_size
    cbz     w2, break

loop:
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, loop

break:
    // jump to C code
    bl      kmain
    // should not return but halt just in case
    b       halt
