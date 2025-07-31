#pragma once

#include <types.h>


typedef u32 sl_t;


extern void sl_acquire(sl_t *sl);
extern void sl_release(sl_t *sl);
