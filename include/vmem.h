#ifndef _VMEM_H
#define _VMEM_H

#include <types.h>


// 0xffff... -> ttbr0 -> kernel
// 0x0000... -> ttbr1 -> user
#define KERNEL_BASE         0xffff000000000000

#define P2V(paddr)  ((u64)(paddr) + KERNEL_BASE)
#define V2P(vaddr)  ((u64)(vaddr) - KERNEL_BASE)


// extract the table index (0 - 512) from a virtual address
#define INDEX_L0(vaddr)     (((vaddr) >> 39) & 0x1ff)
#define INDEX_L1(vaddr)     (((vaddr) >> 30) & 0x1ff)
#define INDEX_L2(vaddr)     (((vaddr) >> 21) & 0x1ff)
#define INDEX_L3(vaddr)     (((vaddr) >> 12) & 0x1ff)


// 4kb, 2mb, 1gb
#define PAGE_SIZE           0x1000
#define LARGE_PAGE_SIZE     0x200000
#define BLOCK_SIZE          0x40000000

#define PTE_PT              0b11    // another page table
#define PTE_PAGE            0b11    // 4kb
#define PTE_BLOCK           0b01    // 2mb, 1gb

#define PTE_USER            (1 << 6)
#define PTE_READONLY        (1 << 7)
#define PTE_ACCESSED        (1 << 10)
#define PTE_NO_EXEC         (1ul << 54)

#define PTE_OSH             (0b10 << 8)
#define PTE_ISH             (0b11 << 8)

// index of the device type in the mair_elx
#define PTE_MEM             (0 << 2)
#define PTE_DEV             (1 << 2)


extern u64 *pt0_kernel;
extern u64 *pt0_user;

void vmem_bootstrap(void);

void vmem_map(u64 vaddr, u64 paddr);

#endif 
