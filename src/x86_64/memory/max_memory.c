#include <stdint.h>
#include "max_memory.h"

#define MEMORY_START 0x00100000  // 1 MiB
#define MEMORY_STEP  (1024 * 1024) // 1 MiB krok
#define MAX_POSSIBLE_MEM (4ULL * 1024 * 1024 * 1024) // Maks. 4 GiB

volatile uint32_t *const test_ptr = (uint32_t*)0x1000; // tymczasowy adres do testowania

uint64_t get_max_memory() {
    uint32_t size = MEMORY_START;

    while (size < MAX_POSSIBLE_MEM) {
        volatile uint32_t *addr = (uint32_t*)(uintptr_t)size;

        uint32_t old = *addr;
        *addr = 0xA5A5A5A5;

        if (*addr != 0xA5A5A5A5) {
            break;
        }

        *addr = old; // przywróć starą wartość
        size += MEMORY_STEP;
    }

    return size;
}
