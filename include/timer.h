#pragma once

#include <types.h>


void sleep_cycles(u64 n_cycles);
void sleep_qs(u64 qs);

void timer_init(u8 n, u32 delay_ms, u8 target);
void timer_handle_irq(u8 n);
