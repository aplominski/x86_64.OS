#include "heap.h"
#include <stddef.h>
#include <stdint.h>
void* malloc(size_t size) {
    if (size == 0) return NULL;
    size = ALIGN4(size);

    BlockHeader* current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
            // Podziel blok jeśli jest dużo większy niż potrzebujemy
            if (current->size >= size + sizeof(BlockHeader) + 4) {
                BlockHeader* new_block = (BlockHeader*)((uint8_t*)current + sizeof(BlockHeader) + size);
                new_block->size = current->size - size - sizeof(BlockHeader);
                new_block->next = current->next;
                new_block->free = 1;

                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void*)(current + 1); // Dane po nagłówku
        }
        current = current->next;
    }
    return NULL; // Brak miejsca
}