#pragma once

#include <types.h>
#include <fs/mbr.h>


typedef enum {
    DRIVE_SD
} drive_type_t;

typedef struct {
    drive_type_t type;
    bool (*init)(void);
    bool (*read)(void *dest, u32 lba, u32 n_secs);
    bool (*write)(void *src, u32 lba, u32 n_secs);
} drive_t;
