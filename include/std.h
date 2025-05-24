#ifndef _STD_H
#define _STD_H

#include <types.h>


void mem_set(u8 *dest, u8 val, u64 n_bytes);
void mem_cpy(u8 *dest, u8 *src, u64 n_bytes);

#endif
