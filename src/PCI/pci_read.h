#ifndef __PCI_READ_H
#define __PCI_READ_H

#include <stdint.h>

// Odczytuje Vendor ID urządzenia PCI (0x00 offset)
uint16_t pcie_read_vendor_id(uint8_t bus, uint8_t device, uint8_t function);

// Odczytuje Device ID urządzenia PCI (0x00 offset)
uint16_t pcie_read_device_id(uint8_t bus, uint8_t device, uint8_t function);

// Odczytuje Class Code urządzenia PCI (0x08 offset, górny bajt)
uint8_t pcie_read_class_code(uint8_t bus, uint8_t device, uint8_t function);

// Odczytuje Subclass urządzenia PCI (0x08 offset, drugi bajt)
uint8_t pcie_read_subclass(uint8_t bus, uint8_t device, uint8_t function);

// Odczytuje typ nagłówka urządzenia PCI
uint8_t pcie_read_header_type(uint8_t bus, uint8_t device, uint8_t function);

uint32_t pcie_config_read32(uint32_t address);
#endif