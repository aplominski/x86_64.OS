#include "ramfs.h"
#include "../../heap/heap.h"

uint8_t *ramfs_read_content_uint8_t(ramfs_file_t *file, size_t *size) {
    if (file == NULL || file->content == NULL || size == NULL) {
        return NULL; // Invalid file or size pointer
    }

    // Allocate memory for the content
    uint8_t *content_copy = (uint8_t *)malloc(file->size);
    if (content_copy == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy the content to the new memory location
    for (size_t i = 0; i < file->size; i++) {
        content_copy[i] = file->content[i];
    }

    // Set the size to the size of the file
    *size = file->size;

    return content_copy; // Return the copied content
}

char *ramfs_read_content_string(ramfs_file_t *file, size_t *size) {
    if (file == NULL || file->content == NULL || size == NULL) {
        return NULL; // Invalid file or size pointer
    }

    // Allocate memory for the content
    char *content_copy = (char *)malloc(file->size + 1); // +1 for null terminator
    if (content_copy == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy the content to the new memory location
    for (size_t i = 0; i < file->size; i++) {
        content_copy[i] = file->content[i];
    }
    
    content_copy[file->size] = '\0'; // Null-terminate the string

    // Set the size to the size of the file
    *size = file->size;

    return content_copy; // Return the copied content
}