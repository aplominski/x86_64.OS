#ifndef __RAMFS_H
#define __RAMFS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
typedef struct {
    char *name;        // Name of the file
    uint8_t *content; // Pointer to the file content in memory
    size_t size; // Size of the file in bytes
} ramfs_file_t;

typedef struct ramfs_dir_t ramfs_dir_t; // Forward declaration of ramfs_dir_t

struct ramfs_dir_t {
    char *name;        // Name of the directory
    size_t file_count; // Number of files in the directory
    ramfs_file_t *files; // Array of files in the directory
    ramfs_dir_t *subdirs; // Array of subdirectories
    size_t subdir_count; // Number of subdirectories in the directory
    struct ramfs_dir_t *parent; // Pointer to the parent directory
};

typedef struct {

    ramfs_dir_t *root; // Pointer to the root directory of the RAMFS
    size_t total_size; // Total size of the RAMFS in bytes
    size_t used_size;  // Used size of the RAMFS in bytes

    bool compression_enabled; // Flag to indicate if compression is enabled
    size_t compression_level; // Compression level (if applicable)
    bool encryption_enabled; // Flag to indicate if encryption is enabled
    uint8_t *encryption_key; // Pointer to the encryption key (if applicable) (sha512 hash)

} ramfs_t;

// Low Level RAMFS API
void ramfs_init(ramfs_dir_t *root, size_t size);
void ramfs_destroy(ramfs_dir_t *dir);

ramfs_file_t *ramfs_create_file(ramfs_dir_t *dir, const char *name, const uint8_t *content, size_t size);
ramfs_dir_t *ramfs_create_dir(ramfs_dir_t *parent, const char *name);

bool ramfs_delete_file(ramfs_dir_t *dir, const char *name);
bool ramfs_delete_dir(ramfs_dir_t *dir, const char *name);

ramfs_file_t *ramfs_get_file(ramfs_dir_t *dir, const char *name);
ramfs_dir_t *ramfs_get_dir(ramfs_dir_t *dir, const char *name); 

void save_ramfs_uint8(ramfs_file_t *file, const uint8_t *content, size_t size);
uint8_t *ramfs_read_content_uint8_t(ramfs_file_t *file, size_t *size);

void save_ramfs_string(ramfs_file_t *file, const char *content, size_t size);
char *ramfs_read_content_string(ramfs_file_t *file, size_t *size);

void ramfs_save_to_file(ramfs_t *ramfs, const char *filename);
ramfs_t *ramfs_load_from_file(const char *filename);

#endif