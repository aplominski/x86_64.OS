#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>
// === KONFIGURACJA STERTY ===
#define HEAP_SIZE   (1024 * 1024) // 1 MiB sterty
static uint8_t heap_area[HEAP_SIZE];

// Struktura nagłówka każdego bloku
typedef struct BlockHeader {
    size_t size;
    struct BlockHeader* next;
    int free;
} BlockHeader;

#define ALIGN4(x) (((x) + 3) & ~3)

static BlockHeader* free_list = NULL;

inline void heap_init() {
    free_list = (BlockHeader*)heap_area;
    free_list->size = HEAP_SIZE - sizeof(BlockHeader);
    free_list->next = NULL;
    free_list->free = 1;
}
void* malloc(size_t size);
void free(void* ptr);
void* calloc(size_t nmemb, size_t size);
void* realloc(void* ptr, size_t new_size);

#endif
