#pragma once

#include <types.h>


void irq_init(void);

void irq_handle(void);
void irq_handle_invalid(u64 vec, u64 esr, u64 elr, u64 far);
