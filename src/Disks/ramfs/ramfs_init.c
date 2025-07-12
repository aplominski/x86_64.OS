#include "ramfs.h"
#include "../../heap/heap.h"

void ramfs_init(ramfs_dir_t *root, size_t size) {
    if (root == NULL) {
        return; // Cannot initialize a null root directory
    }

    // Initialize the root directory
    root->name = "root";
    root->file_count = 0;
    root->files = NULL; // No files initially
    root->subdirs = NULL; // No subdirectories initially
    root->subdir_count = 0;
    root->parent = NULL; // Root directory has no parent

    // Initialize the RAMFS structure
    ramfs_t *ramfs = (ramfs_t *)malloc(sizeof(ramfs_t));
    if (ramfs == NULL) {
        return; // Memory allocation failed
    }

    ramfs->root = root;
    ramfs->total_size = size;
    ramfs->used_size = 0;
    ramfs->compression_enabled = false; // Compression is disabled by default
    ramfs->compression_level = 0; // Default compression level
    ramfs->encryption_enabled = false; // Encryption is disabled by default
    ramfs->encryption_key = NULL; // No encryption key initially
}