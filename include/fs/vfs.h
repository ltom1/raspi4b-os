#pragma once

#include <types.h>


typedef struct {
    char name[8];
    char ext[8];
	u8 attr;

    u8 create_100ms; 
	u16 create_time;
	u16 create_date;
	u16 access_date;

	u16 modified_time;
	u16 modified_date;

    u32 filesize;
    u8 *data;
} file_t;


const char *vfs_get_basename(const char *path);
