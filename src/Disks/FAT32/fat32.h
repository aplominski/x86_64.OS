#ifndef __FAT32_H_
#define __FAT32_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------
// 📂 FAT32 Constants & Macros
// ----------------------------

#define FAT32_ENTRY_DELETED      0xE5
#define FAT32_ENTRY_FREE         0x00
#define FAT32_ATTR_READ_ONLY     0x01
#define FAT32_ATTR_HIDDEN        0x02
#define FAT32_ATTR_SYSTEM        0x04
#define FAT32_ATTR_VOLUME_ID     0x08
#define FAT32_ATTR_DIRECTORY     0x10
#define FAT32_ATTR_ARCHIVE       0x20
#define FAT32_ATTR_LFN           (FAT32_ATTR_READ_ONLY | FAT32_ATTR_HIDDEN | FAT32_ATTR_SYSTEM | FAT32_ATTR_VOLUME_ID)

#define FAT32_CLUSTER_END        0x0FFFFFF8
#define FAT32_CLUSTER_BAD        0x0FFFFFF7

#define FAT32_MAX_FILENAME       256

// ----------------------------
// 📦 BIOS Parameter Block (BPB)
// ----------------------------

#pragma pack(push, 1)
typedef struct {
    uint8_t  jump[3];
    char     oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  fat_count;
    uint16_t root_dir_entries;  // 0 for FAT32
    uint16_t total_sectors_16;
    uint8_t  media_descriptor;
    uint16_t sectors_per_fat_16;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;

    // FAT32 Extended
    uint32_t sectors_per_fat_32;
    uint16_t flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fs_info_sector;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved2;
    uint8_t  boot_signature;
    uint32_t volume_id;
    char     volume_label[11];
    char     fs_type[8];
} FAT32_BPB_t;

// 📁 FAT32 Directory Entry
typedef struct {
    char     name[11]; // 8.3 format
    uint8_t  attr;
    uint8_t  nt_reserved;
    uint8_t  creation_time_tenths;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t write_time;
    uint16_t write_date;
    uint16_t first_cluster_low;
    uint32_t file_size;
} FAT32_DirEntry_t;
#pragma pack(pop)

// ----------------------------
// 🔧 Public API
// ----------------------------

int fat32_mount(uint32_t lba_start);
int fat32_list_dir(uint32_t cluster);
int fat32_read_file(const char* filename, uint8_t* buffer, size_t max_size);
int fat32_find(const char* filename, FAT32_DirEntry_t* out_entry);

// ----------------------------
// 📡 Low-level Access (to implement)
// ----------------------------

int fat32_read_sector(uint32_t lba, uint8_t* buffer); // Musisz zaimplementować
int fat32_read_cluster(uint32_t cluster, uint8_t* buffer);
uint32_t fat32_next_cluster(uint32_t current_cluster);

// ----------------------------
// 🌐 Global State (Optional)
// ----------------------------

extern FAT32_BPB_t fat32_bpb;
extern uint32_t fat32_data_start_sector;
extern uint32_t fat32_fat_start_sector;

#ifdef __cplusplus
}
#endif

#endif // __FAT32_H_
