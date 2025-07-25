#pragma once

#include <types.h>
#include <hw/mmap.h>


// 0xffff... -> ttbr0 -> kernel
// 0x0000... -> ttbr1 -> user
#define KERNEL_BASE         0xffff000000000000

#define P2V(paddr)      ((u64)(paddr) + KERNEL_BASE)
#define V2P(vaddr)      ((u64)(vaddr) - KERNEL_BASE)
#define P2GPU(addr)     ((u64)(addr) | VC_UNCACHED)
#define GPU2P(addr)     ((u64)(addr) & 0x3fffffff)

// extract the table index (0 - 512) from a virtual address
#define INDEX_L0(vaddr)     (((vaddr) >> 39) & 0x1ff)
#define INDEX_L1(vaddr)     (((vaddr) >> 30) & 0x1ff)
#define INDEX_L2(vaddr)     (((vaddr) >> 21) & 0x1ff)
#define INDEX_L3(vaddr)     (((vaddr) >> 12) & 0x1ff)

// mask off all flags
#define PTE(pte)            ((pte) & 0x1ffffffffff000) 
#define PTE_FLAGS(pte)      ((pte) & ~0x1ffffffffff000) 

#define PAGE_NO(paddr)      (PTE(paddr) >> 12)


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
#define PTE_NG              (1 << 11)
#define PTE_PXN             (1ul << 53)
#define PTE_UXN             (1ul << 54)
#define PTE_SHARED          (1ul << 55)

#define PTE_COW             (PTE_SHARED | PTE_READONLY)

#define PTE_OSH             (0b10 << 8)
#define PTE_ISH             (0b11 << 8)

// index of the device type in the mair_elx
#define PTE_MEM             (0 << 2)
#define PTE_DEV             (1 << 2)

#define N_PTE               512

#define TTBR_ASID_SHIFT     48


extern u64 *pt0_kernel;
extern u64 *pt0_user;

void vmem_bootstrap(void);
u64 *vmem_copy_address_space(u64 *old_pt0);
void vmem_destroy_address_space(u64 *pt0);
void vmem_handle_page_fault(u64 far);

void vmem_map_page(u64 *pt0, u64 vaddr, u64 paddr, u64 flags);
void vmem_map(u64 *pt0, u64 vaddr, u64 paddr, u64 n_bytes, u64 flags);

void mmu_enable(void);
void mmu_disable(void);

void mmu_activate_address_space(void);
void mmu_change_pt(u64 ttbr0_paddr, u64 ttbr1_paddr);
void mmu_mair_init(void);
void mmu_tcr_init(void);
