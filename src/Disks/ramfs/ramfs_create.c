#include "ramfs.h"
#include "../../heap/heap.h"
#include "../../utils/string.h"

ramfs_file_t *ramfs_create_file(ramfs_dir_t *dir, const char *name, const uint8_t *content, size_t size) {
    if (!dir || !name || size == 0) {
        return NULL; // Invalid parameters
    }

    // Check if the file already exists
    for (size_t i = 0; i < dir->file_count; i++) {
        if (strcmp(dir->files[i].name, name) == 0) {
            return NULL; // File already exists
        }
    }

    // Allocate memory for the new file
    ramfs_file_t *new_file = (ramfs_file_t *)malloc(sizeof(ramfs_file_t));
    if (!new_file) {
        return NULL; // Memory allocation failed
    }

    // Initialize the new file
    new_file->name = (char *)malloc(strlen(name) + 1);
    if (!new_file->name) {
        free(new_file);
        return NULL; // Memory allocation failed
    }
    strcpy(new_file->name, name);

    new_file->content = (uint8_t *)malloc(size);
    if (!new_file->content) {
        free(new_file->name);
        free(new_file);
        return NULL; // Memory allocation failed
    }
    memcpy(new_file->content, content, size);
    
    new_file->size = size;

    // Add the new file to the directory
    dir->files = (ramfs_file_t *)realloc(dir->files, sizeof(ramfs_file_t) * (dir->file_count + 1));
    if (!dir->files) {
        free(new_file->content);
        free(new_file->name);
        free(new_file);
        return NULL; // Memory allocation failed
    }
    
    dir->files[dir->file_count] = *new_file;
    dir->file_count++;

    free(new_file); // Free the temporary new file pointer

    return &dir->files[dir->file_count - 1]; // Return the newly created file
}

ramfs_dir_t *ramfs_create_dir(ramfs_dir_t *parent, const char *name) {
    if (!parent || !name) {
        return NULL; // Invalid parameters
    }

    // Check if the directory already exists
    for (size_t i = 0; i < parent->subdir_count; i++) {
        if (strcmp(parent->subdirs[i].name, name) == 0) {
            return NULL; // Directory already exists
        }
    }

    // Allocate memory for the new directory
    ramfs_dir_t *new_dir = (ramfs_dir_t *)malloc(sizeof(ramfs_dir_t));
    if (!new_dir) {
        return NULL; // Memory allocation failed
    }

    // Initialize the new directory
    new_dir->name = (char *)malloc(strlen(name) + 1);
    if (!new_dir->name) {
        free(new_dir);
        return NULL; // Memory allocation failed
    }
    strcpy(new_dir->name, name);

    new_dir->file_count = 0;
    new_dir->files = NULL;
    new_dir->subdir_count = 0;
    new_dir->subdirs = NULL;
    new_dir->parent = parent;

    // Add the new directory to the parent directory
    parent->subdirs = (ramfs_dir_t *)realloc(parent->subdirs, sizeof(ramfs_dir_t) * (parent->subdir_count + 1));
    if (!parent->subdirs) {
        free(new_dir->name);
        free(new_dir);
        return NULL; // Memory allocation failed
    }

    parent->subdirs[parent->subdir_count] = *new_dir;
    parent->subdir_count++;

    free(new_dir); // Free the temporary new directory pointer

    return &parent->subdirs[parent->subdir_count - 1]; // Return the newly created directory
}