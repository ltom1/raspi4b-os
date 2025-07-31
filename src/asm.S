.global asm_get_core
asm_get_core:
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    ret


.global asm_change_sp
asm_change_sp:
    mov     sp, x0
    ret

.global asm_get_tcr
asm_get_tcr:
    mrs     x0, tcr_el1
    ret

.global asm_get_sctlr
asm_get_sctlr:
    mrs     x0, sctlr_el1
    ret


.global asm_get_sp
asm_get_sp:
    mov     x0, sp
    ret
.global asm_get_spsel
asm_get_spsel:
    mrs     x0, spsel
    ret

.global asm_get_time
asm_get_time:
    mrs     x0, cntpct_el0
    ret
.global asm_get_timer_freq
asm_get_timer_freq:
    mrs     x0, cntfrq_el0
    ret
.global asm_set_cntkctl
asm_set_cntkctl:
    msr     cntkctl_el1, x0
    ret


.global asm_halt
asm_halt:
    wfe
    b       asm_halt

.global asm_irq_enable
asm_irq_enable:
   	msr     daifclr, #2 
	ret

.global asm_irq_disable
asm_irq_disable:
    msr	    daifset, #2
	ret

.global asm_tlb_invld
asm_tlb_invld:
    lsr     x0, x0, #12
    dsb     ishst
    tlbi    vaae1is, x0
	dsb     ish
	isb
	ret

.global asm_tlb_invld_asid
asm_tlb_invld_asid:
    dsb     ishst
    tlbi    aside1is, x0
	dsb     ish
	isb
	ret

.global asm_tlb_invld_all
asm_tlb_invld_all:
    dsb     ishst
    tlbi    vmalle1is
	dsb     ish
	isb
	ret




// synchronize instruction and data access
.global asm_barrier
asm_barrier:
	dsb     ish
	isb
	ret

.global asm_cache_clean
asm_cache_clean:
    // Disable L1 Caches
MRS X0, SCTLR_EL1 // Read SCTLR_EL3.
BIC X0, X0, #(0x1 << 2) // Disable D Cache.
MSR SCTLR_EL1, X0 // Write SCTLR_EL3.
// Invalidate Data cache to make the code general purpose.
// Calculate the cache size first and loop through each set +
// way.
MOV X0, #0x0 // X0 = Cache level
MSR CSSELR_EL1, x0 // 0x0 for L1 Dcache 0x2 for L2 Dcache.
MRS X4, CCSIDR_EL1 // Read Cache Size ID.
AND X1, X4, #0x7
ADD X1, X1, #0x4 // X1 = Cache Line Size.
LDR X3, =0x7FFF
AND X2, X3, X4, LSR #13 // X2 = Cache Set Number – 1.
LDR X3, =0x3FF
AND X3, X3, X4, LSR #3 // X3 = Cache Associativity Number – 1.
CLZ W4, W3 // X4 = way position in the CISW instruction.
MOV X5, #0 // X5 = way counter way_loop.
way_loop:
MOV X6, #0 // X6 = set counter set_loop.
set_loop:
LSL X7, X5, X4
    ret

.global asm_cache_invld
asm_cache_invld:
    add     x1, x1, x0      // base Address + length

    // clean the data cache by mva

    mrs     x2, ctr_el0     // read cache type register
    
    // get the minimun data cache line
    ubfx    x4, x2, #16, #4 // extract dminline (log2 of the cache line)
    mov     x3, #4          // dminline iss the number of words (4 bytes)
    lsl     x3, x3, x4      // x3 should contain the cache line
    sub     x4, x3, #1      // get the mask for the cache line
    bic     x4, x0, x4      // aligned the base address of the region
clean_data_cache:
    dc      cvau, x4        // clean data cache line by va to PoU
    add     x4, x4, x3      // next cache line
    cmp     x4, x1          // is x4 (current cache line) smaller than the end
                            // of the region

    b.lt    clean_data_cache // while (address < end_address)
    dsb     ish              // ensure visibility of the data cleaned from cache

    //clean the instruction cache by va
    
    // get the minimum instruction cache line (x2 contains ctr_el0)
    and     x2, x2, #0xf    // extract iminline (log2 of the cache line)
    mov     x3, #4          // iminline is the number of words (4 bytes)
    lsl     x3, x3, x2      // x3 should contain the cache line
    sub     x4, x3, #1      // get the mask for the cache line
    bic     x4, x0, x4      // aligned the base address of the region
clean_instruction_cache:
    ic      ivau, x4        // clean instruction cache line by va to pou
    add     x4, x4, x3      // next cache line
    cmp     x4, x1          // is x4 (current cache line) smaller than the end
                            // of the region
    b.lt clean_instruction_cache // while (address < end_address)
    dsb ish                 // ensure completion of the invalidations
    isb                     // synchronize the fetched instruction strea
    ret
