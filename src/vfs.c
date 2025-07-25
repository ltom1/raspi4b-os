#include <types.h>
#include <fs/vfs.h>


const char *vfs_get_basename(const char *path) {

    u64 last_slash = 0;
    u64 i = 0;
    while (path[i]) {
        if (path[i] == '/') last_slash = i + 1;
        i++;
    }

    return &path[last_slash];
}
