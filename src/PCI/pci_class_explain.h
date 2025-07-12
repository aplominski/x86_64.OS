// pci_class_explain.h
#ifndef PCI_CLASS_EXPLAIN_H
#define PCI_CLASS_EXPLAIN_H

#include <stdint.h>

// Opis klas PCI (class codes)
static const char* PCI_CLASS_DESCRIPTIONS[256] = {
    [0x00] = "Unclassified device",
    [0x01] = "Mass Storage Controller",
    [0x02] = "Network Controller",
    [0x03] = "Display Controller",
    [0x04] = "Multimedia Controller",
    [0x05] = "Memory Controller",
    [0x06] = "Bridge Device",
    [0x07] = "Simple Communication Controller",
    [0x08] = "Base System Peripheral",
    [0x09] = "Input Device Controller",
    [0x0A] = "Docking Station",
    [0x0B] = "Processor",
    [0x0C] = "Serial Bus Controller",
    [0x0D] = "Wireless Controller",
    [0x0E] = "Intelligent I/O Controller",
    [0x0F] = "Satellite Communication Controller",
    [0x10] = "Encryption Controller",
    [0x11] = "Signal Processing Controller",
    [0x12] = "Processing Accelerator",
    [0x13] = "Non-Essential Instrumentation",
    [0x14] = "Management Controller",
    [0x15] = "Reserved",
    [0xFF] = "Unknown Class"
};

// Mass Storage Controller (0x01)
static const char* PCI_SUBCLASS_01[128] = {
    [0x00] = "SCSI Bus Controller",
    [0x01] = "IDE Controller",
    [0x02] = "Floppy Disk Controller",
    [0x03] = "IPI Bus Controller",
    [0x04] = "RAID Controller",
    [0x05] = "ATA Controller",
    [0x06] = "Serial ATA",
    [0x07] = "Serial Attached SCSI",
    [0x08] = "Non-Volatile Memory Controller"
};

// Network Controller (0x02)
static const char* PCI_SUBCLASS_02[129] = {
    [0x00] = "Ethernet Controller",
    [0x01] = "Token Ring Controller",
    [0x02] = "FDDI Controller",
    [0x03] = "ATM Controller",
    [0x04] = "ISDN Controller",
    [0x05] = "WorldFip Controller",
    [0x06] = "PICMG Controller",
    [0x80] = "Other Network Controller",
};

// Display Controller (0x03)
static const char* PCI_SUBCLASS_03[128] = {
    [0x00] = "VGA Compatible Controller",
    [0x01] = "XGA Controller",
    [0x02] = "3D Controller",
};

// Bridge Device (0x06)
static const char* PCI_SUBCLASS_06[128] = {
    [0x00] = "Host Bridge",
    [0x01] = "ISA Bridge",
    [0x02] = "EISA Bridge",
    [0x03] = "MCA Bridge",
    [0x04] = "PCI-to-PCI Bridge",
    [0x05] = "PCI-to-CardBus Bridge",
    [0x06] = "PCI-to-PCI Bridge (second definition)",
    [0x07] = "MCA Bridge (second definition)",
};

// Serial Bus Controller (0x0C)
static const char* PCI_SUBCLASS_0C[128] = {
    [0x00] = "FireWire (IEEE 1394) Controller",
    [0x01] = "ACCESS Bus Controller",
    [0x02] = "SSA Controller",
    [0x03] = "USB Controller",
    [0x04] = "Fibre Channel Controller",
    [0x05] = "SMBus Controller",
    [0x06] = "InfiniBand Controller",
    [0x07] = "IPMI Interface",
    [0x08] = "SERCOS Interface",
    [0x09] = "CANbus Controller",
};

// Wireless Controller (0x0D)
static const char* PCI_SUBCLASS_0D[128] = {
    [0x00] = "iRDA Compatible Controller",
    [0x01] = "Consumer IR Controller",
    [0x10] = "RF Controller",
    [0x11] = "Bluetooth Controller",
    [0x12] = "Broadband Controller",
    [0x20] = "Ethernet Controller (802.11a)",
    [0x21] = "Ethernet Controller (802.11b)",
};

// Domyślny opis dla nieznanych podklas
static const char* PCI_SUBCLASS_UNKNOWN = "Unknown subclass";

// Funkcja pomocnicza do pobierania opisu podklasy
static inline const char* pci_get_subclass_description(uint8_t class_code, uint8_t subclass) {
    switch (class_code) {
        case 0x01:
            return PCI_SUBCLASS_01[subclass] ? PCI_SUBCLASS_01[subclass] : PCI_SUBCLASS_UNKNOWN;
        case 0x02:
            return PCI_SUBCLASS_02[subclass] ? PCI_SUBCLASS_02[subclass] : PCI_SUBCLASS_UNKNOWN;
        case 0x03:
            return PCI_SUBCLASS_03[subclass] ? PCI_SUBCLASS_03[subclass] : PCI_SUBCLASS_UNKNOWN;
        case 0x06:
            return PCI_SUBCLASS_06[subclass] ? PCI_SUBCLASS_06[subclass] : PCI_SUBCLASS_UNKNOWN;
        case 0x0C:
            return PCI_SUBCLASS_0C[subclass] ? PCI_SUBCLASS_0C[subclass] : PCI_SUBCLASS_UNKNOWN;
        case 0x0D:
            return PCI_SUBCLASS_0D[subclass] ? PCI_SUBCLASS_0D[subclass] : PCI_SUBCLASS_UNKNOWN;
        default:
            return PCI_SUBCLASS_UNKNOWN;
    }
}

#endif // PCI_CLASS_EXPLAIN_H
