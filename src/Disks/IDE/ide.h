#ifndef __IDE_H
#define __IDE_H

#define ATA_PRIMARY_IO     0x1F0
#define ATA_PRIMARY_CTRL   0x3F6
#define ATA_SECONDARY_IO   0x170
#define ATA_SECONDARY_CTRL 0x376

#define ATA_REG_DATA       0
#define ATA_REG_ERROR      1
#define ATA_REG_FEATURES   1
#define ATA_REG_SECCOUNT   2
#define ATA_REG_LBA0       3
#define ATA_REG_LBA1       4
#define ATA_REG_LBA2       5
#define ATA_REG_HDDEVSEL   6
#define ATA_REG_COMMAND    7
#define ATA_REG_STATUS     7

#define ATA_CMD_IDENTIFY          0xEC
#define ATA_CMD_IDENTIFY_PACKET   0xA1

typedef struct {
    char type[16]; 
    char model[41];
    int channel; // 0 for primary, 1 for secondary
    int drive;   // 0 for master, 1 for slave
} IDEDevice_t;


void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void io_wait(void);
void copy_model_str(char* dest, const uint16_t* id); 

IDEDevice_t* ide_detect_all(void);
IDEDevice_t ide_detect_device(uint16_t io, uint16_t ctrl, int channel, int drive);

inline int ata_wait() {
    while (inb(ATA_PRIMARY_IO + 7) & 0x80); 
    return 0;
}
#endif