#include "ramfs.h"
#include "../../heap/heap.h"

void save_ramfs_uint8(ramfs_file_t *file, const uint8_t *content, size_t size) {
    if (file == NULL || content == NULL || size == 0) {
        return; // Invalid parameters
    }

    // Allocate memory for the file content
    file->content = (uint8_t *)malloc(size);
    if (file->content == NULL) {
        return; // Memory allocation failed
    }

    // Copy the content to the file
    for (size_t i = 0; i < size; i++) {
        file->content[i] = content[i];
    }
    
    file->size = size; // Set the size of the file
}

void save_ramfs_string(ramfs_file_t *file, const char *content, size_t size) {
    if (file == NULL || content == NULL || size == 0) {
        return; // Invalid parameters
    }

    // Allocate memory for the file content
    file->content = (uint8_t *)malloc(size + 1); // +1 for null terminator
    if (file->content == NULL) {
        return; // Memory allocation failed
    }

    // Copy the content to the file and add null terminator
    for (size_t i = 0; i < size; i++) {
        file->content[i] = (uint8_t)content[i];
    }
    file->content[size] = '\0'; // Null-terminate the string

    file->size = size + 1; // Set the size of the file including null terminator
}