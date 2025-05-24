#pragma once

#include <types.h>
#include <hw/mmap.h>


#define AUX_ENABLE      ((u32*)(AUX_BASE + 0x04))


typedef struct {
    u32 io;
    u32 ier;
    u32 iir;
    u32 lcr;
    u32 mcr;
    u32 lsr;
    u32 msr;
    u32 scratch;
    u32 ctrl;
    u32 stat;
    u32 baud;
} aux_mu_t;


#define AUX_MU          ((aux_mu_t*)(AUX_BASE + 0x40))
