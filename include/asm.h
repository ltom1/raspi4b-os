#pragma once

#include <types.h>
#include <hw/sysregs.h>


#define ASM   __asm__ __volatile__


extern u8 asm_get_core(void);
extern tcr_t asm_get_tcr(void);
extern u64 asm_get_sctlr(void);

extern u64 asm_get_sp(void);
extern u64 asm_get_spsel(void);

extern u64 asm_get_time(void);
extern u64 asm_get_timer_freq(void);
extern void asm_set_cntkctl(u64 val);

extern void asm_tlb_invld(u64 vaddr);
extern void asm_tlb_invld_asid(u8 asid);
extern void asm_tlb_invld_all(void);

extern void asm_barrier(void);
extern void asm_cache_invld(u64 start, u64 size);
extern void asm_cache_clean(void);

extern void asm_halt(void);
extern void asm_test(void);
extern void asm_irq_enable(void);
extern void asm_irq_disable(void);
