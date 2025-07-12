#include <stddef.h>
#include <stdint.h>
#include "../../utils/string.h"
#include "ide.h"
#include "../../console.h"

void uitoa(unsigned num, char* out) {
    char buf[10];
    int i = 0;
    if (num == 0) {
        out[0] = '0';
        out[1] = 0;
        return;
    }
    while (num > 0) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    for (int j = 0; j < i; j++)
        out[j] = buf[i - j - 1];
    out[i] = 0;
}

static void parse_model(char model_out[41], uint16_t* id) {
    for (int i = 0; i < 20; ++i) {
        model_out[i * 2]     = (id[27 + i] >> 8) & 0xFF;
        model_out[i * 2 + 1] = id[27 + i] & 0xFF;
    }
    model_out[40] = '\0';
    // Usuwanie końcowych spacji:
    for (int i = 39; i >= 0; --i) {
        if (model_out[i] == ' ' || model_out[i] == '\0')
            model_out[i] = '\0';
        else
            break;
    }
}

IDEDevice_t ide_detect_device(uint16_t io, uint16_t ctrl, int channel, int drive) {
    outb(io + ATA_REG_HDDEVSEL, 0xA0 | (drive << 4));
    io_wait();

    outb(io + ATA_REG_SECCOUNT, 0);
    outb(io + ATA_REG_LBA0, 0);
    outb(io + ATA_REG_LBA1, 0);
    outb(io + ATA_REG_LBA2, 0);

    outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    io_wait();

    uint8_t status = inb(io + ATA_REG_STATUS);
    if (status == 0) {
        IDEDevice_t device = {0};
        strncpy(device.type, "None", sizeof(device.type) - 1);
        strncpy(device.model, "None", sizeof(device.model) - 1);
        device.channel = channel;
        device.drive = drive;
        return device;
    }

    uint8_t lba1 = inb(io + ATA_REG_LBA1);
    uint8_t lba2 = inb(io + ATA_REG_LBA2);

    IDEDevice_t device = {0};
    device.channel = channel;
    device.drive = drive;

    if (lba1 == 0x14 && lba2 == 0xEB) {
        // ATAPI
        outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY_PACKET);
        io_wait();
        status = inb(io + ATA_REG_STATUS);
        if (status == 0) {
            strncpy(device.type, "None", sizeof(device.type) - 1);
            strncpy(device.model, "None", sizeof(device.model) - 1);
            return device;
        }
        while ((status & 0x80) && !(status & 0x08)) {
            status = inb(io + ATA_REG_STATUS);
        }

        uint16_t id[256];
        for (int i = 0; i < 256; ++i)
            id[i] = inw(io + ATA_REG_DATA);

        strncpy(device.type, "ATAPI", sizeof(device.type) - 1);
        parse_model(device.model, id);

        return device;
    } else if (lba1 == 0x00 && lba2 == 0x00) {
        // ATA
        while ((status & 0x80) && !(status & 0x08)) {
            status = inb(io + ATA_REG_STATUS);
        }

        uint16_t id[256];
        for (int i = 0; i < 256; ++i)
            id[i] = inw(io + ATA_REG_DATA);

        strncpy(device.type, "ATA", sizeof(device.type) - 1);
        parse_model(device.model, id);

        return device;
    } else {
        strncpy(device.type, "Unknown", sizeof(device.type) - 1);
        strncpy(device.model, "Unknown", sizeof(device.model) - 1);
        device.channel = -1;
        device.drive = -1;
        return device;
    }
}
IDEDevice_t* ide_detect_all(void) {
    static IDEDevice_t devices[4];
    devices[0] = ide_detect_device(ATA_PRIMARY_IO, ATA_PRIMARY_CTRL, 0, 0);   // Primary Master
    devices[1] = ide_detect_device(ATA_PRIMARY_IO, ATA_PRIMARY_CTRL, 0, 1);   // Primary Slave
    devices[2] = ide_detect_device(ATA_SECONDARY_IO, ATA_SECONDARY_CTRL, 1, 0); // Secondary Master
    devices[3] = ide_detect_device(ATA_SECONDARY_IO, ATA_SECONDARY_CTRL, 1, 1); // Secondary Slave
    return devices;
}
