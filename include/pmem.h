#pragma once

#include <types.h>


extern u8 *page_ref;

void pmem_init(void);

u64 pmem_alloc(u64 n_bytes);
void pmem_free(u64 paddr, u64 n_bytes);

bool pmem_bitmap_get_block(u64 block);
void pmem_bitmap_mark_block(u64 block, bool used);
void pmem_bitmap_mark_blocks(u64 block, u64 count, bool used);
