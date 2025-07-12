#include "heap.h"
void* realloc(void* ptr, size_t new_size) {
    if (!ptr) return malloc(new_size);
    if (new_size == 0) {
        free(ptr);
        return NULL;
    }

    BlockHeader* block = (BlockHeader*)ptr - 1;
    if (block->size >= new_size) {
        return ptr;
    }

    void* new_ptr = malloc(new_size);
    if (!new_ptr) return NULL;

    size_t copy_size = block->size < new_size ? block->size : new_size;
    uint8_t* dst = (uint8_t*)new_ptr;
    uint8_t* src = (uint8_t*)ptr;

    for (size_t i = 0; i < copy_size; ++i) {
        dst[i] = src[i];
    }

    free(ptr);
    return new_ptr;
}
