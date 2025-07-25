#include <types.h>
#include <dbg.h>
#include <std.h>
#include <alloc.h>
#include <buddy.h>
#include <pmem.h>
#include <vmem.h>
#include <asm.h>


u64 buddy_alloc(alloc_t *alloc, u64 n_bytes) {

    buddy_alloc_t *self = (buddy_alloc_t*)alloc;

    u8 *bitmap;
    u64 layer;
    u64 layer_off;
    u64 layer_size;
    bool buddy1;
    bool buddy2;

    u64 best_layer = buddy_layer_from_size(n_bytes);
    if (best_layer == 0) {
        dbg_info("Buddy alloc: requested allocation size exceeds half page limit\n");
        asm_irq_disable();
        asm_halt();
    }

    for (u64 page = 0; page < self->alloc.blocks; page++) {
    
        bitmap = (u8*)self->alloc.base_addr + page * PAGE_SIZE;

        layer = best_layer;

        for (; layer > 0; layer--) {
        
            layer_off = buddy_bit_offset_layer(layer);
            layer_size = buddy_bits_in_layer(layer);

            for (u64 off = layer_off; off < layer_off + layer_size; off += 2) {
                // check for a pair with different values
                buddy1 = buddy_bitmap_get_bit(bitmap, off);
                buddy2 = buddy_bitmap_get_bit(bitmap, off + 1);
                if (!(buddy1 ^ buddy2)) continue;

                // if true mark the unused buddy as used
                off += buddy1;
                buddy_bitmap_mark_bit(bitmap, off, true);
                off -= layer_off;

                // check if we are on the correct layer
                // loop through all layers and mark the correct blocks as used
                while (layer != best_layer) {
                    layer++;
                    off *= 2;
                    buddy_bitmap_mark_bit(bitmap, buddy_bit_offset_layer(layer) + off, true);
                }

                // calculate and return start address
                return self->alloc.base_addr + off * buddy_layer_block_size(layer);
            }
        }
    }

    dbg_info("Buddy alloc: out of memory\n");
    asm_irq_disable();
    asm_halt();

    return 0;
}


u64 buddy_layer_from_size(u64 n_bytes) {

    if (n_bytes > GREATEST / 2) {
        dbg_info("Buddy alloc: requested allocation size exceeds half page limit\n");
        asm_irq_disable();
        asm_halt();
    }
    
    u64 layer;
    if (n_bytes <= SMALLEST) layer = LAYERS - 1;
    else {
        u64 size = GREATEST;
        for (layer = 0; layer < LAYERS; layer++) {
            if (size / n_bytes == 1) break;
            size /= 2;
        }
    }
    return layer;
}


u64 buddy_layer_block_size(u64 layer) {

    u64 size = GREATEST;
    for (u64 i = 0; i < layer; i++) {
        size /= 2;
    }
    return size;
}


bool buddy_init(alloc_t *alloc) {

    buddy_alloc_t *self = (buddy_alloc_t*)alloc;

    u8 *bitmap;

    // allocate some space at the beginning of every page for bitmaps
    for (u64 i = 0; i < self->alloc.blocks; i++) {

        bitmap = (u8*)self->alloc.base_addr + i * PAGE_SIZE;
        for (u64 layer = 0; layer <= 5; layer++) {
            buddy_bitmap_mark_bit(bitmap, buddy_bit_offset_layer(layer), true);
        }
    }

    return true;
}


void buddy_bitmap_mark_bits(u8 *bitmap, u64 off, u64 count, bool val) {

    for (; off % 8 && count > 0; ++off, --count)
        buddy_bitmap_mark_bit(bitmap, off, val);
 
    if (count >= 8) {

        mem_set(&bitmap[off / 8], val * -1, count / 8);
        off += (count - count % 8);
        count = count % 8;
    }

    for (; count > 0; ++off, --count)
        buddy_bitmap_mark_bit(bitmap, off, val);
}


void buddy_bitmap_mark_bit(u8 *bitmap, u64 off, bool val) {

    u64 byte = off / 8;
    u64 bit = off % 8;

    if (val) {
        bitmap[byte] |= (1 << (7 - bit));
    } else {
        bitmap[byte] &= ~(1 << (7 - bit));
    }
}


bool buddy_bitmap_get_bit(u8 *bitmap, u64 bit) {

    u64 byte = bit / 8;
    u64 off = bit % 8;

    return (bitmap[byte] >> (7 - off)) & 1;
}


u64 buddy_bit_offset_layer(u64 layer) {

    return math_pow(2, layer) - 1;
}


u64 buddy_bits_in_layer(u64 layer) {

    return math_pow(2, layer);
}


void buddy_free(alloc_t *alloc, u64 vaddr, u64 n_bytes) {

    buddy_alloc_t *self = (buddy_alloc_t*)alloc;

    u64 off;
    bool block;

    u64 off_from_base = vaddr - self->alloc.base_addr;
    u64 page_num = off_from_base / PAGE_SIZE;
    u8 *bitmap = (u8*)self->alloc.base_addr + page_num * PAGE_SIZE;
    off_from_base %= PAGE_SIZE;

    for (
            u64 layer = LAYERS - 1; 
            (off_from_base % buddy_layer_block_size(layer) == 0) && (layer > 0); 
            layer--) {

        off = off_from_base / buddy_layer_block_size(layer);
        block = buddy_bitmap_get_bit(bitmap, buddy_bit_offset_layer(layer) + off);

        if (!block) continue;

        u64 size = buddy_layer_block_size(layer);
        mem_set((u8*)vaddr, 0, size);

        buddy_bitmap_mark_bit(bitmap, buddy_bit_offset_layer(layer) + off, false);

        if (off % 2 == 1) off--;
        else off++;
        
        block = buddy_bitmap_get_bit(bitmap, buddy_bit_offset_layer(layer) + off);

        if (block) return;
        while (layer != 0) {
        
            off /= 2;
            layer--;

            buddy_bitmap_mark_bit(bitmap, buddy_bit_offset_layer(layer) + off, false);

            if (off % 2 == 1) off--;
            else off++;

            block = buddy_bitmap_get_bit(bitmap, buddy_bit_offset_layer(layer) + off);

            if (block) return;
            
        }
        break;

    }

    dbg_info("Buddy alloc: there is no block used\n");
    asm_irq_disable();
    asm_halt();
}


void buddy_visualize_bitmap(u8* bitmap) {

    u64 size = 2;
    for (u64 off = 0; off <= TOTAL_BLOCKS; off++) {

        if (off % size == size - 1) {
            size *= 2;
            dbg_info("\n");
        }
        dbg_info("%c", buddy_bitmap_get_bit(bitmap, off) ? 'X' : 'O');
    }
}
