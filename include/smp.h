#pragma once

#include <types.h>


#define N_CORES 4

extern u64 *spin_core;
extern u64 kstacks[N_CORES];

extern void _start(void);


void smp_core_stub(void);
void smp_init(void);
void smp_core_init(u8 core);
