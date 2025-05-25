#pragma once

#include <types.h>


typedef struct PACKED TCR {
    u64 t0sz    : 6;
    u64 _0      : 2;
    u64 irgn0   : 2;
    u64 orgn0   : 2;
    u64 sh0     : 2;
    u64 tg0     : 2;

    u64 t1sz    : 6;
    u64 _1      : 2;
    u64 irgn1   : 2;
    u64 orgn1   : 2;
    u64 sh1     : 2;
    u64 tg1     : 2;

    u64 ips_size: 3;
    u64 _3      : 2;
    u64 tbi     : 2;

    u64 _4      : 25;
} tcr_t;


// normal memory
// -------------
// Normal memory, Outer Write-Back Non-transient
// Normal memory, Inner Write-Back Non-transient
// Inner Read-Allocate policy: Allocate
// Inner Write-Allocate policy: Allocate
#define MAIR_MEM            0xff

// device memory
// -------------
// Device-nGnRE memory
#define MAIR_DEV            0x04
