#pragma once

#include <types.h>


typedef struct Allocator {
    u64 base_addr;
    u64 blocks;
    u64 space_left;
    u64 (*alloc)(struct Allocator *self, u64 n_bytes);
    void (*free)(struct Allocator *self, u64 vaddr, u64 n_bytes);
    bool (*init)(struct Allocator *self);
} alloc_t;
