#include "pci_read.h"
#include "pci_utils.h"
uint16_t pcie_read_vendor_id(uint8_t bus, uint8_t device, uint8_t function) {
    outl(PCI_CONFIG_ADDRESS, pci_config_address(bus, device, function, 0x00));
    return inl(PCI_CONFIG_DATA) & 0xFFFF;
}

uint16_t pcie_read_device_id(uint8_t bus, uint8_t device, uint8_t function) {
    outl(PCI_CONFIG_ADDRESS, pci_config_address(bus, device, function, 0x00));
    return (inl(PCI_CONFIG_DATA) >> 16) & 0xFFFF;
}

uint8_t pcie_read_class_code(uint8_t bus, uint8_t device, uint8_t function) {
    outl(PCI_CONFIG_ADDRESS, pci_config_address(bus, device, function, 0x08));
    return (inl(PCI_CONFIG_DATA) >> 24) & 0xFF;
}

uint8_t pcie_read_subclass(uint8_t bus, uint8_t device, uint8_t function) {
    outl(PCI_CONFIG_ADDRESS, pci_config_address(bus, device, function, 0x08));
    return (inl(PCI_CONFIG_DATA) >> 16) & 0xFF;
}

uint8_t pcie_read_header_type(uint8_t bus, uint8_t device, uint8_t function) {
    uint32_t address;
    uint32_t lbus = (uint32_t)bus;
    uint32_t ldevice = (uint32_t)device;
    uint32_t lfunction = (uint32_t)function;

    // Formularz adresu konfiguracji PCI (standardowy)
    address = (uint32_t)((1 << 31)              // enable bit
              | (lbus << 16)
              | (ldevice << 11)
              | (lfunction << 8)
              | (0x0E & 0xFC));               // offset 0x0E aligned do 4 bajtów (0x0C) - odczyt będzie 32-bitowy

    // Załóżmy, że masz funkcję pcie_config_read32(address), która odczytuje 32 bity
    uint32_t data = pcie_config_read32(address);

    // header type jest w bajcie o offsetcie 0x0E czyli w bits 16-23 32-bitowego słowa (bo 0x0C jest aligned)
    uint8_t header_type = (data >> 16) & 0xFF;
    return header_type;
}
uint32_t pcie_config_read32(uint32_t address) {
    outl(0xCF8, address);
    return inl(0xCFC);
}