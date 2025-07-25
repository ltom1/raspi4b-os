#pragma once

#include <types.h>
#include <alloc.h>
#include <vmem.h>


typedef struct {
    alloc_t alloc;
    u64 top;
} bump_alloc_t;


#define CREATE_BUMP_ALLOCATOR(blocks) (bump_alloc_t) { \
        (alloc_t) { \
            P2V(pmem_alloc(blocks * PAGE_SIZE)), \
            blocks, \
            blocks * PAGE_SIZE, \
            bump_alloc, \
            bump_free, \
            0 \
        }, 0 \
    }; \


u64 bump_alloc(alloc_t *alloc, u64 n_bytes);
void bump_free(alloc_t *alloc, u64 vaddr, u64 n_bytes);
