#pragma once

#include <types.h>


bool emmc_init(void);
bool emmc_read_blocks(void *dest, u32 lba, u32 n_secs);
bool emmc_write_blocks(void *src, u32 lba, u32 n_secs);
