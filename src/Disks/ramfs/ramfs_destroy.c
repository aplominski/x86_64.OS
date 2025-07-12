#include "ramfs.h"
#include "../../heap/heap.h"

void ramfs_destroy(ramfs_dir_t *dir) {
    if (dir == NULL) {
        return; // Nothing to destroy
    }

    // Free all files in the directory
    for (size_t i = 0; i < dir->file_count; i++) {
        ramfs_file_t *file = &dir->files[i];
        if (file->name) {
            free(file->name); // Free the file name
        }
        if (file->content) {
            free(file->content); // Free the file content
        }
    }
    free(dir->files); // Free the array of files

    // Recursively destroy subdirectories
    for (size_t i = 0; i < dir->subdir_count; i++) {
        ramfs_destroy(&dir->subdirs[i]);
    }
    free(dir->subdirs); // Free the array of subdirectories

    // Free the directory name and the directory itself
    if (dir->name) {
        free(dir->name);
    }
    free(dir);
}
