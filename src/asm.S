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

.global asm_halt
asm_halt:
    wfe
    b       asm_halt

.global asm_irq_enable
asm_irq_enable:
   	msr    daifclr, #2 
	ret

.global asm_irq_disable
asm_irq_disable:
    msr	daifset, #2
	ret

.global asm_test
asm_test:
    .word 0xdeadbeef
    ret
