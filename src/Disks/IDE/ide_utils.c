#include <stdint.h>

#include "ide.h"
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void io_wait(void) {
    outb(0x80, 0);
}

void copy_model_str(char* dest, const uint16_t* id) {
    for (int i = 0; i < 20; ++i) {
        dest[i*2]     = (id[27 + i] >> 8) & 0xFF;
        dest[i*2 + 1] = id[27 + i] & 0xFF;
    }
    dest[40] = 0;
}
