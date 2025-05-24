#ifndef _ASM_H
#define _ASM_H

#include <types.h>
#include <mmu.h>


#define ASM   __asm__ __volatile__


extern tcr_t asm_get_tcr(void);
extern u64 asm_get_sctlr(void);

extern void asm_halt(void);
extern void asm_test(void);
extern void asm_irq_enable(void);
extern void asm_irq_disable(void);

#endif
