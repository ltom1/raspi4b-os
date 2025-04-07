#include <types.h>
#include <timer.h>
#include <asm.h>


void sleep_cycles(u64 n_cycles) {
    while (n_cycles--) 
        ASM("nop" : : : "memory");
}
