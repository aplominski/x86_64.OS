#include "heap.h"
void* calloc(size_t nmemb, size_t size) {
    size_t total = nmemb * size;
    void* ptr = malloc(total);
    if (ptr) {
        uint8_t* p = (uint8_t*)ptr;
        for (size_t i = 0; i < total; ++i) {
            p[i] = 0;
        }
    }
    return ptr;
}

