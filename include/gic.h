#pragma once

#include <types.h>


void gic_enable(void);
void gic_disable(void);

void gic_int_enable(u64 n);
void gic_int_disable(u64 n);
