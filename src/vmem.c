#include <types.h>
#include <vmem.h>
#include <layout.h>
#include <asm.h>


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
        | PTE_PT
        | PTE_ACCESSED
        | PTE_ISH
        | PTE_MEM; 

    // identity map the entire RAM
    for (u64 i = 0; i < RAM_SIZE_GB; i++) 
         pt1_user[i] = (i * BLOCK_SIZE)
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
            | PTE_NO_EXEC
            | PTE_OSH
            | PTE_DEV;
}
