#ifndef __PCI_DEVICE_H
#define __PCI_DEVICE_H

#include <stdint.h>
#include <stddef.h>

#include "pci_read.h"
typedef struct {
    uint8_t bus;
    uint8_t device;
    uint8_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_code;
    uint8_t subclass;
} PCIDevice_t;


size_t pcie_scan_bus(PCIDevice_t* out_devices, size_t max_devices);
size_t pcie_get_device_count(void);

#endif