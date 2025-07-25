#pragma once

#include <types.h>


#define MBR_ATTR_BOOTABLE   0x80
#define N_PARTITIONS        4


typedef struct PACKED {
    u8 c;
    u8 h;
    u8 s;
} chs_t;


typedef struct PACKED {
    u8 attr;
    chs_t chs_start;
    u8 type;
    chs_t chs_end;
    u32 lba_start;
    u32 n_secs;
} part_tbl_entry_t;


typedef struct PACKED {
    u8 code[440];
    u32 disk_id;
    u16 res0;
    part_tbl_entry_t part[N_PARTITIONS];
    u16 sign;
} mbr_t;
