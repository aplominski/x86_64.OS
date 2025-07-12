// fat32_low.c
#include "fat32.h"
#include "../../utils/string.h"
#include "../Disks.h"
#include "../IDE/ide.h"

FAT32_BPB_t fat32_bpb;
uint32_t fat32_fat_start_sector = 0;
uint32_t fat32_data_start_sector = 0;

#define SECTOR_SIZE 512
static uint8_t _fat_sector_buffer[SECTOR_SIZE];

int fat32_read_sector(uint32_t lba, uint8_t* buffer) {
    if(kspace_actual_storage_device == NULL) {
        return -1; // No device selected
    }
    if(kspace_actual_storage_device->type == STORAGE_IDE) {
        ata_wait();

        outb(ATA_PRIMARY_CTRL, 0x00);
        outb(ATA_PRIMARY_IO + 2, 1);                // sector count
        outb(ATA_PRIMARY_IO + 3, (uint8_t)lba);     // LBA low
        outb(ATA_PRIMARY_IO + 4, (uint8_t)(lba >> 8));
        outb(ATA_PRIMARY_IO + 5, (uint8_t)(lba >> 16));
        outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F)); // LBA + master
        outb(ATA_PRIMARY_IO + 7, 0x20);             // READ SECTORS

        ata_wait();

        uint16_t* buf16 = (uint16_t*)buffer;
        for (int i = 0; i < 256; ++i)
            buf16[i] = inw(ATA_PRIMARY_IO);

        return 0; //gud gud
    }

    return -2; //unsuported storage type
}

int fat32_mount(uint32_t lba_start) {
    uint8_t sector[SECTOR_SIZE];
    if (fat32_read_sector(lba_start, sector) != 0)
        return -1;

    memcpy(&fat32_bpb, sector, sizeof(FAT32_BPB_t));

    fat32_fat_start_sector = lba_start + fat32_bpb.reserved_sectors;
    fat32_data_start_sector = fat32_fat_start_sector +
                              fat32_bpb.fat_count * fat32_bpb.sectors_per_fat_32;

    return 0;
}


int fat32_read_cluster(uint32_t cluster, uint8_t* buffer) {
    if (cluster < 2)
        return -1;

    uint32_t first_sector = fat32_data_start_sector +
                            (cluster - 2) * fat32_bpb.sectors_per_cluster;

    for (uint8_t i = 0; i < fat32_bpb.sectors_per_cluster; ++i) {
        if (fat32_read_sector(first_sector + i, buffer + (i * fat32_bpb.bytes_per_sector)) != 0)
            return -1;
    }

    return 0;
}
