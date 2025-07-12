#include "heap.h"
#include <stddef.h>
#include <stdint.h>
void free(void* ptr) {
    if (!ptr) return;
    BlockHeader* block = (BlockHeader*)ptr - 1;
    block->free = 1;

    // Scalanie sąsiednich wolnych bloków
    BlockHeader* current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += sizeof(BlockHeader) + current->next->size;
            current->next = current->next->next;
        } else {
            current = current->next;
        }
    }
}