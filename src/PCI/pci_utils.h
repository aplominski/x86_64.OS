#ifndef __PCI_UTILS_
#define __PCI_UTILS_

#include <stdint.h>

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC


void outl(uint16_t port, uint32_t val);
uint32_t inl(uint16_t port);
uint32_t pci_config_address(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);

#endif