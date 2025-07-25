#include <types.h>
#include <alloc.h>
#include <pmem.h>
#include <bump.h>


u64 bump_alloc(alloc_t *alloc, u64 n_bytes) {

    bump_alloc_t *self = (bump_alloc_t*)alloc;

    u64 vaddr = self->alloc.base_addr + self->top;
    self->top += n_bytes;
    return vaddr;
}


void bump_free(alloc_t *alloc, u64 vaddr, u64 n_bytes) {

    bump_alloc_t *self = (bump_alloc_t*)alloc;

    self->top -= n_bytes;
}
