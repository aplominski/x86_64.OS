#include "ramfs.h"
#include "../../heap/heap.h"
#include "../../utils/string.h"

bool ramfs_delete_file(ramfs_dir_t *dir, const char *name) {
    if (dir == NULL || name == NULL || dir->files == NULL) {
        return false; // Invalid directory or name
    }

    for (size_t i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i].name, name) == 0) {
            // Free the file content memory
            free(dir->files[i].content);
            // Shift remaining files to fill the gap
            for (size_t j = i; j < dir->file_count - 1; j++) {
                dir->files[j] = dir->files[j + 1];
            }
            // Decrease the file count
            dir->file_count--;
            return true; // File deleted successfully
        }
    }
    return false; // File not found
}

bool ramfs_delete_dir(ramfs_dir_t *dir, const char *name) {
    if (dir == NULL || name == NULL || dir->subdirs == NULL) {
        return false; // Invalid directory or name
    }

    for (size_t i = 0; i < dir->subdir_count; i++) {
        if (strcmp(dir->subdirs[i].name, name) == 0) {
            // Recursively delete all files in the subdirectory
            for (size_t j = 0; j < dir->subdirs[i].file_count; j++) {
                free(dir->subdirs[i].files[j].content);
            }
            // Free the subdirectory's files array
            free(dir->subdirs[i].files);
            // Free the subdirectory's name
            free(dir->subdirs[i].name);
            // Shift remaining subdirectories to fill the gap
            for (size_t j = i; j < dir->subdir_count - 1; j++) {
                dir->subdirs[j] = dir->subdirs[j + 1];
            }
            // Decrease the subdirectory count
            dir->subdir_count--;
            return true; // Directory deleted successfully
        }
    }
    return false; // Directory not found
}