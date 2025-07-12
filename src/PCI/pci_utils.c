#include "pci_utils.h"
void outl(uint16_t port, uint32_t val) {
    __asm__ volatile ("outl %0, %1" : : "a"(val), "Nd"(port));
}

uint32_t inl(uint16_t port) {
    uint32_t ret;
    __asm__ volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

uint32_t pci_config_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    return (uint32_t)(
        (1U << 31) |                // enable bit
        ((bus & 0xFF) << 16) |      // bus number
        ((device & 0x1F) << 11) |   // device number
        ((function & 0x07) << 8) |  // function number
        (offset & 0xFC)             // offset (must be aligned to 4)
    );
}
