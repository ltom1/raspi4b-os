#pragma once

#include <types.h>
#include <buddy.h>
#include <drive.h>
#include <alloc.h>
#include <fs/mbr.h>
#include <fs/vfs.h>


#define FAT32_EOF               0x0ffffff8
#define FAT32_NAME              8
#define FAT32_EXT               3

#define FAT32_READONLY          0x01
#define FAT32_HIDDEN            0x02
#define FAT32_SYSTEM            0x03
#define FAT32_DIR               0x10
#define FAT32_ARCHIVE           0x20

     
// structure of the volume boot record containing a BIOS Parameter Block
// contains file system information
typedef struct PACKED {

    u8  jmp[3];
    u8  oem_id[8];
    u16 bytes_per_sector;
    u8  secs_per_cluster;
    u16 n_reserved_secs;
    u8  n_fats;
    u16 n_root_entries;
    u16 fat12_n_secs;
    u8  media_descriptor;

    // IMPORTANT: not used by FAT32
    u16 fat12_secs_per_fat;

    u16 secs_per_track;
    u16 secs_per_head;
    u32 lba_partition;
    u32 fat12_n_secs_large;

    // IMPORTANT: used by FAT32
    u32 fat32_secs_per_fat;

    u16 flags;
    u16 fs_version;
    u32 cluster_root;
    u16 lba_fsinfo;
    u16 lba_backup_bootsector;

    u8  reserved[12];

    u8  drive_num;
    u8  reserved_byte;
    u8  signature;
    u32 volume_id;
    u8  volume_label[11];
    u8  system_id[8];

    u8 code[420];
    u16 sign;

} vbr_t;


// structure of an entry in a FAT32 directory cluster
typedef struct PACKED {
	char name[8];
	char ext[3];
	u8 attr;
	u8 reserved;
 
    u8 create_100ms; 
	u16 create_time;
	u16 create_date;
	u16 access_date;
	u16 cluster_high;
 
	u16 modified_time;
	u16 modified_date;
	u16 cluster_low;
	u32 filesize;
} fat32_dir_entry_t;


#define FAT_ENTRIES_PER_PAGE    1024
#define DIR_ENTRIES_PER_PAGE    128

#define N_FATS_CACHED   2
#define N_DIRS_CACHED   2

#define SECTOR_SIZE     512


typedef struct {
   
    vbr_t vbr;
    drive_t *drive;
    part_tbl_entry_t *part;

    // holds the nth cluster of the fat for each cache slot
    u32 fats_cached[N_FATS_CACHED];
    u8 *cache_fat;

    // holds the cluster of the dir for each cache slot
    u32 dirs_cached[N_DIRS_CACHED];
    u8 *cache_dir;

    u8 *cache_root;

    u32 fat_entries_per_cluster;
    u32 dir_entries_per_cluster;

    u8 secs_per_cluster;
    u32 secs_per_fat;
    u64 lba_data;
    u64 lba_fat;
    u32 cluster_root;

} fat32_t;


extern fat32_t *fs;
extern buddy_alloc_t heap;


fat32_t *fat32_init(alloc_t *alloc, drive_t *drive, part_tbl_entry_t *part);
file_t* fat32_load_file(fat32_t *fs, alloc_t *alloc, const char *filepath);
