#include <types.h>
#include <elf64.h>
#include <proc.h>
#include <dbg.h>
#include <asm.h>
#include <pmem.h>
#include <std.h>
#include <vmem.h>


bool elf64_check(file_t *f) {

    elf64_header_t *elf = (elf64_header_t*)f->data;

    if ((elf->ident[0] != ELF_MAGIC0) ||
        (elf->ident[1] != ELF_MAGIC1) ||
        (elf->ident[2] != ELF_MAGIC2) ||
        (elf->ident[3] != ELF_MAGIC3)) {
        dbg_info("ELF: invalid Magic\n");
        return false;
    }
    
    if (elf->ident[ELF_IDENT_CLASS] != ELF_CLASS64) {
        dbg_info("ELF: not a 64bit executable");
        return false;
    }
    
    if (elf->ident[ELF_IDENT_ORDER] != ELF_ORDER_LE) {
        dbg_info("ELF: byte order not little endian\n");
        return false;
    }

    if (elf->ident[ELF_IDENT_VERSION] != ELF_VERSION1) {
        dbg_info("ELF: wrong elf version\n");
        return false;
    }

    if (elf->type != ELF_TYPE_EXE) {
        dbg_info("ELF: not an executable\n");
        return false;
    }

    if (elf->arch != ELF_ARCH_AARCH64) {
        dbg_info("ELF: not an aarch64 executable\n");
        return false;
    }

    return true;
}


u64 elf64_extract(pcb_t *proc) {

    u64 highest_seg = 0;
    u64 stack_vaddr = 0;

    elf64_header_t *elf = (elf64_header_t*)proc->file->data;
    elf64_pheader_t *pheaders = (elf64_pheader_t*)(proc->file->data + elf->pht_off);
    
    for (u64 i = 0; i < elf->pht_entries; i++) {

        if (pheaders[i].size_mem != pheaders[i].size_file) {
            dbg_info("Additional memory has to be allocated!\n");
        }
        if (pheaders[i].type != SEG_LOAD) continue;
        
        dbg_info("%u: %x - %x\n", pheaders[i].type, pheaders[i].vaddr, pheaders[i].vaddr + pheaders[i].size_mem);
        
        // place the stack after the highest segment
        if (pheaders[i].vaddr > highest_seg) {
            highest_seg = pheaders[i].vaddr;
            stack_vaddr = (pheaders[i].vaddr + pheaders->size_mem + PAGE_SIZE - 1) & ~0xfff;
        }

        vmem_map(
            proc->pt0, 
            pheaders[i].vaddr, 
            V2P(proc->file->data) + pheaders[i].off, 
            pheaders[i].size_mem,
            PTE_USER
        );

        // resolve cache issues
        // cache thinks we are loading data 
        // -> instruction cache doesn't hold our loaded instructions on real hardware 
        // -> clean and invalidate caches
        asm_barrier();
        asm_cache_clean();
        asm_barrier();
    }

    // allocate and map stack
    u64 stack = P2V(pmem_alloc(STACK_SIZE));
    vmem_map(proc->pt0, stack_vaddr, V2P(stack), STACK_SIZE, PTE_USER);

    return stack_vaddr;
}
