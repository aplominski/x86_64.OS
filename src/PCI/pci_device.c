#include "pci_device.h"
#include "pci_read.h"

#define PCI_MAX_BUS     256
#define PCI_MAX_DEVICE  32
#define PCI_MAX_FUNCTION 8

static size_t _pcie_device_count = 0;

size_t pcie_scan_bus(PCIDevice_t* out_devices, size_t max_devices) {
    size_t count = 0;

    for (uint8_t bus = 0; bus < PCI_MAX_BUS; ++bus) {
        for (uint8_t device = 0; device < PCI_MAX_DEVICE; ++device) {
            for (uint8_t function = 0; function < PCI_MAX_FUNCTION; ++function) {

                uint16_t vendor = pcie_read_vendor_id(bus, device, function);
                if (vendor == 0xFFFF) {
                    // Brak urządzenia pod tym BDF
                    if (function == 0) {
                        // Jeśli funkcja 0 nie istnieje, to cały device można pominąć
                        break;
                    }
                    continue;
                }

                if (count >= max_devices)
                    return count;

                PCIDevice_t* dev = &out_devices[count++];
                dev->bus = bus;
                dev->device = device;
                dev->function = function;
                dev->vendor_id = vendor;
                dev->device_id = pcie_read_device_id(bus, device, function);
                dev->class_code = pcie_read_class_code(bus, device, function);
                dev->subclass = pcie_read_subclass(bus, device, function);

                // Sprawdzenie, czy urządzenie jest multi-function
                if (function == 0) {
                    uint8_t header_type = pcie_read_header_type(bus, device, 0);
                    if ((header_type & 0x80) == 0) {
                        // Nie jest multi-function, pomiń pozostałe funkcje
                        break;
                    }
                }
            }
        }
    }

    _pcie_device_count = count;
    return count;
}

size_t pcie_get_device_count(void) {
    return _pcie_device_count;
}
