#include <std.h>
#include <types.h>
#include <vmem.h>
#include <pmem.h>
#include <hw/mmap.h>
#include <asm.h>
#include <dbg.h>
#include <proc.h>


// provided by the linker
extern u8 _end;

// page tables placed after the kernel
u64 *pt0_kernel = (u64*)(&_end + 0x0000);
u64 *pt0_user = (u64*)(&_end + 0x3000);


void vmem_bootstrap(void) {

    u64 *pt0_kernel = (u64*)(&_end + 0x0000);
    u64 *pt1_kernel = (u64*)(&_end + 0x1000);
    u64 *pt2_kernel = (u64*)(&_end + 0x2000);

    u64 *pt0_user = (u64*)(&_end + 0x3000);
    u64 *pt1_user = (u64*)(&_end + 0x4000);

    // identity mapping (temporary)

    pt0_user[0] = ((u64)pt1_user)
        | PTE_NG
        | PTE_PT
        | PTE_ACCESSED
        | PTE_ISH
        | PTE_MEM; 

    // identity map the entire RAM
    for (u64 i = 0; i < RAM_SIZE_GB; i++) 
         pt1_user[i] = (i * BLOCK_SIZE)
            | PTE_NG
            | PTE_BLOCK
            | PTE_ACCESSED
            | PTE_ISH
            | PTE_MEM;

    pt0_kernel[0] = ((u64)pt1_kernel)
        | PTE_PT
        | PTE_ACCESSED
        | PTE_ISH
        | PTE_MEM; 

    // identity map the entire RAM
    for (u64 i = 0; i < RAM_SIZE_GB; i++) 
         pt1_kernel[i] = (i * BLOCK_SIZE)
            | PTE_BLOCK
            | PTE_ACCESSED
            | PTE_ISH
            | PTE_MEM;


    // device memory map
    pt1_kernel[INDEX_L1(MMIO_BASE)] = ((u64)pt2_kernel)
        | PTE_PT
        | PTE_ACCESSED
        | PTE_ISH
        | PTE_MEM;

    // set up device memory descriptors (DEV, OSH, NO_EXEC)
    for (u64 i = 0; i < 16; i++)
        pt2_kernel[INDEX_L2(MMIO_BASE) + i] = (MMIO_BASE + i * LARGE_PAGE_SIZE)
            | PTE_BLOCK
            | PTE_ACCESSED
            | PTE_PXN
            | PTE_UXN
            | PTE_OSH
            | PTE_DEV;
}


void vmem_map_page(u64 *pt0, u64 vaddr, u64 paddr, u64 flags) {

    if (!pt0[INDEX_L0(vaddr)]) {
        dbg_info("pt1\n");
        pt0[INDEX_L0(vaddr)] = pmem_alloc(PAGE_SIZE)
            | PTE_NG
            | PTE_PT
            | PTE_ACCESSED
            | PTE_ISH
            | PTE_MEM;
    }

    u64 *pt1 = (u64*)P2V(PTE(pt0[INDEX_L0(vaddr)]));

    if (!pt1[INDEX_L1(vaddr)]) {
        dbg_info("pt2\n");
        pt1[INDEX_L1(vaddr)] = pmem_alloc(PAGE_SIZE)
            | PTE_NG
            | PTE_PT
            | PTE_ACCESSED
            | PTE_ISH
            | PTE_MEM;
    }

    u64 *pt2 = (u64*)P2V(PTE(pt1[INDEX_L1(vaddr)]));

    if (!pt2[INDEX_L2(vaddr)]) {
        dbg_info("pt3\n");
        pt2[INDEX_L2(vaddr)] = pmem_alloc(PAGE_SIZE)
            | PTE_NG
            | PTE_PT
            | PTE_ACCESSED
            | PTE_ISH
            | PTE_MEM;
    }

    u64 *pt3 = (u64*)P2V(PTE(pt2[INDEX_L2(vaddr)]));

    if (pt3[INDEX_L3(vaddr)]) 
             dbg_info("VMEM: warn already mapped %x to %x\n", vaddr, paddr);

    pt3[INDEX_L3(vaddr)] = paddr
        | PTE_NG
        | PTE_PAGE
        | PTE_ACCESSED
        | PTE_ISH
        | PTE_MEM
        | flags;

    page_ref[PAGE_NO(paddr)]++;
}


void vmem_map(u64 *pt0, u64 vaddr, u64 paddr, u64 n_bytes, u64 flags) {

    u64 n_pages = (n_bytes + PAGE_SIZE - 1) / PAGE_SIZE;

    for (u64 i = 0; i < n_pages; i++)
        vmem_map_page(pt0, vaddr + i * PAGE_SIZE, paddr + i * PAGE_SIZE, flags);
}


void vmem_destroy_address_space(u64 *pt0) {

    u64 *pt1;
    u64 *pt2;
    u64 *pt3;

    for (u64 i0 = 0; i0 < N_PTE; i0++) {
        if (!pt0[i0]) continue;

        pt1 = (u64*)P2V(PTE(pt0[i0]));
        for (u64 i1 = 0; i1 < N_PTE; i1++) {
            if (!pt1[i1]) continue;

            pt2 = (u64*)P2V(PTE(pt1[i1] & ~0xfff));
            for (u64 i2 = 0; i2 < N_PTE; i2++) {
                if (!pt2[i2]) continue;

                pt3 = (u64*)P2V(PTE(pt2[i2]));
                for (u64 i3 = 0; i3 < N_PTE; i3++) {
                    if (!pt3[i3]) continue;

                    // if this was the last process using the page
                    if (!(--page_ref[PAGE_NO(PTE(pt3[i3]))]))
                        pmem_free(PTE(pt3[i3]), PAGE_SIZE);

                    // else it is still shared with other processes
                }

                pmem_free(V2P(pt3), PAGE_SIZE);
            }

            pmem_free(V2P(pt2), PAGE_SIZE);
        }
        
         pmem_free(V2P(pt1), PAGE_SIZE);
    }

    pmem_free(V2P(pt0), PAGE_SIZE);
}


u64 *vmem_copy_address_space(u64 *old_pt0) {

    u64 *old_pt1;
    u64 *old_pt2;
    u64 *old_pt3;

    u64 *new_pt0 = (u64*)P2V(pmem_alloc(PAGE_SIZE));
    u64 *new_pt1;
    u64 *new_pt2;
    u64 *new_pt3;
    
    u64 pte_flags = PTE_NG | PTE_PT | PTE_ACCESSED | PTE_ISH | PTE_MEM;

    for (u64 i0 = 0; i0 < N_PTE; i0++) {
        if (!old_pt0[i0]) continue;

        old_pt1 = (u64*)P2V(PTE(old_pt0[i0]));
        new_pt1 = (u64*)pmem_alloc(PAGE_SIZE);
        new_pt0[i0] = (u64)new_pt1 | pte_flags;
        new_pt1 = (u64*)P2V(new_pt1);

        for (u64 i1 = 0; i1 < N_PTE; i1++) {
            if (!old_pt1[i1]) continue;

            old_pt2 = (u64*)P2V(PTE(old_pt1[i1]));
            new_pt2 = (u64*)pmem_alloc(PAGE_SIZE);
            new_pt1[i1] = (u64)new_pt2 | pte_flags;
            new_pt2 = (u64*)P2V(new_pt2);

            for (u64 i2 = 0; i2 < N_PTE; i2++) {
                if (!old_pt2[i2]) continue;

                old_pt3 = (u64*)P2V(PTE(old_pt2[i2]));
                new_pt3 = (u64*)pmem_alloc(PAGE_SIZE);
                new_pt2[i2] = (u64)new_pt3 | pte_flags;
                new_pt3 = (u64*)P2V(new_pt3);

                for (u64 i3 = 0; i3 < N_PTE; i3++) {
                    if (!old_pt3[i3]) continue;

                    // original mapping (not shared)
                    if (!(old_pt3[i3] & PTE_COW)) {
                        old_pt3[i3] |= PTE_COW;
                    }

                    new_pt3[i3] = old_pt3[i3];
                    page_ref[PAGE_NO(PTE(old_pt3[i3]))]++;

                    asm_tlb_invld_asid(cur_proc[asm_get_core()]->pid);
                }
            }
        }
    }

    return new_pt0;
}


// only called on permission fault (copy-on-write) when writing to a readonly page
void vmem_handle_page_fault(u64 far) {

    u64 *pt0 = cur_proc[asm_get_core()]->pt0;
    u64 *pt1 = (u64*)P2V(PTE(pt0[INDEX_L0(far)]));
    u64 *pt2 = (u64*)P2V(PTE(pt1[INDEX_L1(far)]));
    u64 *pt3 = (u64*)P2V(PTE(pt2[INDEX_L2(far)]));

    u64 old_paddr = PTE(pt3[INDEX_L3(far)]);
    u64 old_flags = PTE_FLAGS(pt3[INDEX_L3(far)]);

    // this process is the only one using the page -> not shared anymore
    if (page_ref[PAGE_NO(old_paddr)] == 1) {
        pt3[INDEX_L3(far)] &= ~PTE_COW;
        asm_tlb_invld(far & ~0xfff);
        return;
    }

    // there are two or more processes using this page
    
    // -> create a private copy
    u64 new_paddr = pmem_alloc(PAGE_SIZE);
    mem_cpy((u8*)P2V(new_paddr), (u8*)P2V(old_paddr), PAGE_SIZE);
    // ... which is not shared yet
    pt3[INDEX_L3(far)] = (new_paddr | old_flags) & ~PTE_COW;

    page_ref[PAGE_NO(new_paddr)]++;
    page_ref[PAGE_NO(old_paddr)]--;

    dbg_info("INVALIDATING %x\n", far & ~0xfff);
    asm_tlb_invld(far & ~0xfff);
}
