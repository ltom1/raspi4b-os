#include <types.h>
#include <pmem.h>
#include <layout.h>
#include <vmem.h>
#include <std.h>
#include <dbg.h>
#include <asm.h>


extern u8 _pt_end;

u8 *bitmap;
u64 bitmap_byte_size;
u64 bitmap_bit_size;

u64 next = 0;


void pmem_init(void) {

    bitmap = (u8*)&_pt_end;

    bitmap_bit_size = (RAM_SIZE / PAGE_SIZE) + 1;
    bitmap_byte_size = bitmap_bit_size / 8;

    // set all to unused
    mem_set(bitmap, 0, bitmap_byte_size);

    u64 bitmap_end_block = (V2P(bitmap) + bitmap_byte_size) / PAGE_SIZE;

    // from 0 - end of the bitmap = used
    pmem_bitmap_mark_blocks(0, bitmap_end_block + 1, true);
}


bool pmem_bitmap_get_block(u64 block) {

    u64 byte = block / 8;
    u64 off = block % 8;

    return (bitmap[byte] >> (7 - off)) & 1;
}


void pmem_bitmap_mark_block(u64 block, bool used) {

    u64 byte = block / 8;
    u64 off = block % 8;

    if (used)   bitmap[byte] |= (1 << (7 - off));
    else        bitmap[byte] &= ~(1 << (7 - off));
}


void pmem_bitmap_mark_blocks(u64 block, u64 count, bool used) {

    for (; block % 8 && count > 0; ++block, --count)
        pmem_bitmap_mark_block(block, used);

    // speed things up by writing whole bytes
    if (count >= 8) {

        mem_set(&bitmap[block / 8], used * -1, count / 8);
        block += (count - count % 8);
        count = count % 8;
    }

    // rest
    for (; count > 0; ++block, --count)
        pmem_bitmap_mark_block(block, used);
}


// returns the first block of the found region or -1 
u64 pmem_find_free_region(u64 size) {

    // reset the next if the previous block has been freed
    if (next > 0 && !pmem_bitmap_get_block(next - 1)) next = 0;

    u64 cur_size = 0;
    u64 cur_start = next % bitmap_bit_size;
    u64 cur_block = 0;

    for (u64 i = 0; i < bitmap_bit_size; i++) {

        cur_block = (next + i) % bitmap_bit_size;

        if (cur_block == 0) {
            cur_size = 0;
            cur_start = 0;
        }
        
        if (pmem_bitmap_get_block(cur_block)) {
            cur_size = 0;
            cur_start = cur_block + 1;
        } else {
        
            cur_size++;
            if (cur_size >= size) {
                next = (cur_start + 1) % bitmap_bit_size;
                return cur_start;
            }
        }
    }

    return -1;
}


void pmem_free(u64 paddr, u64 n_blocks) {

    pmem_bitmap_mark_blocks(paddr / PAGE_SIZE, n_blocks, false);
}


// does return the physical address of the allocated block
u64 pmem_alloc(u64 n_blocks) {

    u64 block = pmem_find_free_region(n_blocks);
    if (block == (u64)-1) {
        dbg_info("Out of memory");
        asm_halt();
    }

    pmem_bitmap_mark_blocks(block, n_blocks, true);
    u64 paddr = block * PAGE_SIZE;

    // zero initialize
    mem_set((u8*)P2V(paddr), 0, n_blocks * PAGE_SIZE);

    return paddr;
}
