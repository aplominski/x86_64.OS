#include "ramfs.h"
#include "../../heap/heap.h"
#include "../../utils/string.h"

ramfs_file_t *ramfs_get_file(ramfs_dir_t *dir, const char *name) {
    if (dir == NULL || name == NULL) {
        return NULL; // Invalid directory or name
    }

    for (size_t i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i].name, name) == 0) {
            return &dir->files[i]; // Return the file if found
        }
    }

    return NULL; // File not found
}

ramfs_dir_t *ramfs_get_dir(ramfs_dir_t *dir, const char *name) {
    if (dir == NULL || name == NULL) {
        return NULL; // Invalid directory or name
    }

    for (size_t i = 0; i < dir->subdir_count; i++) {
        if (strcmp(dir->subdirs[i].name, name) == 0) {
            return &dir->subdirs[i]; // Return the directory if found
        }
    }

    return NULL; // Directory not found
}