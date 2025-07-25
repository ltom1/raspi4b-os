#include <types.h>
#include <std.h>


void mem_set(u8 *dest, u8 val, u64 n_bytes) {
  
    for (u64 i = 0; i < n_bytes; i++) {
        *(dest + i) = val;
    }
}


void mem_cpy(u8 *dest, u8 *src, u64 n_bytes) {

    for (u64 i = 0; i < n_bytes; i++) {
        *(dest + i) = *(src + i);
    }
}


u64 math_pow(u64 value, u64 n) {

    u64 res = 1;
    for (u64 i = 0; i < n; i++) {
        res = res * value;
    }

    return res;
}
