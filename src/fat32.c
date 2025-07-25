#include <types.h>
#include <std.h>
#include <vmem.h>
#include <asm.h>
#include <fs/mbr.h>
#include <fs/vfs.h>
#include <fs/fat32.h>
#include <drive.h>
#include <dbg.h>
#include <alloc.h>
#include <pmem.h>
#include <timer.h>


static void fat32_load_cluster(fat32_t *self, u8 *dest, u32 cluster) {

    self->drive->read(
        (void*)dest, 
        self->lba_data + (cluster - 2) * self->secs_per_cluster,  // 2 = first cluster
        self->secs_per_cluster
    );
}


fat32_t *fat32_init(alloc_t *alloc, drive_t *drive, part_tbl_entry_t *part) {

    fat32_t *fat32 = (fat32_t*)alloc->alloc(alloc, sizeof(fat32_t));

    fat32->drive = drive;
    fat32->part = part;

    // read the vbr
    drive->read(&fat32->vbr, part->lba_start, 1);

    
    fat32->lba_fat = fat32->vbr.lba_partition + fat32->vbr.n_reserved_secs;
    fat32->lba_data = fat32->lba_fat + fat32->vbr.n_fats * fat32->vbr.fat32_secs_per_fat;
    fat32->cluster_root = fat32->vbr.cluster_root;
    fat32->secs_per_fat = fat32->vbr.fat32_secs_per_fat;
    fat32->secs_per_cluster = fat32->vbr.secs_per_cluster;

    fat32->fat_entries_per_cluster = (fat32->secs_per_cluster * SECTOR_SIZE) / sizeof(u32);
    fat32->dir_entries_per_cluster = (fat32->secs_per_cluster * SECTOR_SIZE) / sizeof(fat32_dir_entry_t);

    // cache root dir
    fat32->cache_root = (u8*)P2V(pmem_alloc(fat32->secs_per_cluster * SECTOR_SIZE));
    fat32_load_cluster(fat32, fat32->cache_root, fat32->cluster_root);

    // set up fat cache
    mem_set((u8*)&fat32->fats_cached, -1, N_FATS_CACHED * sizeof(u32));
    fat32->cache_fat = (u8*)P2V(pmem_alloc(fat32->secs_per_cluster * SECTOR_SIZE * N_FATS_CACHED));

    // set up dir cache
    mem_set((u8*)&fat32->dirs_cached, -1, N_DIRS_CACHED * sizeof(u32));
    fat32->cache_dir = (u8*)P2V(pmem_alloc(fat32->secs_per_cluster * SECTOR_SIZE * N_DIRS_CACHED));

    return fat32;
}


static u32 fat32_next_cluster(fat32_t *fs, u32 cur_cluster) {

    u64 fat_cluster = cur_cluster / fs->fat_entries_per_cluster;

    // check if the FAT region is cached
    for (u64 i = 0; i < N_FATS_CACHED; i++) {
        if (fs->fats_cached[i] == fat_cluster) {

            return *(u32*)(
                fs->cache_fat 
                + i * fs->secs_per_cluster * SECTOR_SIZE 
                + (cur_cluster % fs->fat_entries_per_cluster) * sizeof(u32)
                );
        }
    }

    // read it from disk otherwise
    for (u64 i = 0; i < N_FATS_CACHED; i++) {
        if (fs->fats_cached[i] != -1) continue;

        fs->fats_cached[i] = fat_cluster;

        fs->drive->read(
                (void*)fs->cache_fat + i * fs->secs_per_cluster * SECTOR_SIZE, 
                fs->lba_fat + fat_cluster * fs->secs_per_cluster, 
                fs->secs_per_cluster
                );
        return *(u32*)(
            fs->cache_fat 
            + i * fs->secs_per_cluster * SECTOR_SIZE 
            + (cur_cluster % fs->fat_entries_per_cluster) * sizeof(u32)
            );
    }

    // if all spots are occupied simply delete the first one
    // todo: better decision making here
    fs->fats_cached[0] = fat_cluster;
    fs->drive->read(
            (void*)fs->cache_fat + 0 * fs->secs_per_cluster * SECTOR_SIZE,
            fs->lba_fat + fat_cluster * fs->secs_per_cluster, 
            fs->secs_per_cluster
            );
    return *(u32*)(
            fs->cache_fat 
            + 0 * fs->secs_per_cluster * SECTOR_SIZE 
            + (cur_cluster % fs->fat_entries_per_cluster) * sizeof(u32)
            );
}


// returns how many clusters were loaded
static u32 fat32_load_cluster_chain(fat32_t *fs, u8 *dest, u32 cluster, u32 max_clusters) {

    u32 cur_cluster = cluster;

    for (u64 i = 0; i < max_clusters; i++) {

 //       dbg_info("Loading cluster %u into %x", cur_cluster, (u64)(dest + i * fs->secs_per_cluster * SECTOR_SIZE));
        fat32_load_cluster(fs, dest + i * fs->secs_per_cluster * SECTOR_SIZE, cur_cluster);;
        cur_cluster = fat32_next_cluster(fs, cur_cluster);
  //      dbg_info(" next %u\n", cur_cluster);
        if (cur_cluster >= FAT32_EOF) return i + 1;
    }
    return max_clusters;
}


// returns a pointer to the cached dir
static u8* fat32_cache_dir(fat32_t *fs, u32 dir_cluster) {

    for (u64 i = 0; i < N_DIRS_CACHED; i++) {

        if (fs->dirs_cached[i] != dir_cluster) continue;

        // already cached
        return fs->cache_dir + i * fs->secs_per_cluster * SECTOR_SIZE;
    }
    // not cached yet (simply put it into the first spot)
    // todo: better decision making here
    fs->dirs_cached[0] = dir_cluster;
    fat32_load_cluster(fs, fs->cache_dir, dir_cluster);
    return fs->cache_dir;
}


// returns how many chars match
static u64 fat32_cmp_path(const char *path_input, const char *path_entry) {

    for (u64 i = 0; i < FAT32_NAME; i++) {
        if (path_input[i] == path_entry[i]) continue;
        
        if ((path_input[i] == '\\') || (path_input[i] == '/') || (path_input[i] == 0)) {
            for (u64 j = i; j < (FAT32_NAME + FAT32_EXT); j++) {
                
                if (path_entry[j] != ' ') return -1;
            }
            return i + 1;
        }
        
        if (path_input[i] == '.') {
            for (u64 j = i; j < FAT32_NAME; j++) {
                
                if (path_entry[j] != ' ') return -1;
            }
            
            i++;
            for (u64 j = FAT32_NAME; j < (FAT32_NAME + FAT32_EXT); ++i, ++j) {
                if (path_input[i] == path_entry[j]) continue;
                
                if ((path_input[i] == '\\') || (path_input[i] == '/') || (path_input[i] == 0)) {
                    
                    for (u64 h = j; h < (FAT32_NAME + FAT32_EXT); h++) {
                
                        if (path_entry[h] != ' ') return -1;
                    }
                    return i + 1;
                }
                return -1;
            }
            
            return i + 1;
        }
        return -1;
    }
    return (FAT32_NAME + FAT32_EXT) + 1;
}


file_t* fat32_load_file(fat32_t *fs, alloc_t *alloc, const char *filepath) {

    fat32_dir_entry_t *entry;
    u64 next_index;
    u32 start_cluster = -1;

    // absolute paths starting from root_dir
    filepath++;
    u8 *cur_dir = fs->cache_root;
    u32 cur_cluster = fs->cluster_root;

repeat:
    for (u64 i = 0; i < fs->dir_entries_per_cluster; i++) {

        entry = (fat32_dir_entry_t*)(cur_dir + i * sizeof(fat32_dir_entry_t));
        next_index = fat32_cmp_path(filepath, entry->name);

        // no match
        if (next_index == -1) continue;

        // match
        // next dir/file of path
        filepath += next_index;

        // entry is the final file
        if (!(entry->attr & FAT32_DIR)) {
            start_cluster = DWORD(entry->cluster_high, entry->cluster_low);
            break;
        }

        // entry is another directory
        cur_cluster = DWORD(entry->cluster_high, entry->cluster_low);

        // cache and select new directory
        cur_dir = fat32_cache_dir(fs, cur_cluster);

        // start from the top in new directory
        i = 0;
    }

    // no entry match
    if (start_cluster == -1) {

        // get next cluster for the current directory
        cur_cluster = fat32_next_cluster(fs, cur_cluster);
        if (cur_cluster >= FAT32_EOF) {
            dbg_info("File could not be found: %s", filepath);
            asm_irq_disable();
            asm_halt();
        }

        // cache and select new cluster
        cur_dir = fat32_cache_dir(fs, cur_cluster);

        goto repeat;
    }

    
    u32 max_clusters = entry->filesize / (fs->secs_per_cluster * SECTOR_SIZE);
    if (entry->filesize % (fs->secs_per_cluster * SECTOR_SIZE)) max_clusters++;
    
    u8 *dest = (u8*)P2V(pmem_alloc(max_clusters * fs->secs_per_cluster * SECTOR_SIZE));

    dbg_info("max_clusters = %u\n", max_clusters);
    u32 clusters_loaded = fat32_load_cluster_chain(fs, dest, start_cluster, max_clusters);
    dbg_info("Clusters loaded: %u\n", clusters_loaded);

    file_t *f = (file_t*)alloc->alloc(alloc, sizeof(file_t));
    f->access_date = entry->access_date;
    f->create_date = entry->create_date;
    f->create_100ms = entry->create_100ms;
    f->create_time = entry->create_time;
    f->modified_date = entry->modified_date;
    f->modified_time = entry->modified_time;
    f->filesize = entry->filesize;
    f->data = dest;
    mem_cpy((u8*)f->name, (u8*)entry->name, FAT32_NAME + FAT32_EXT);

    return f;
}
