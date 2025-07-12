#include "../../utils/string.h"
#include "fat32.h"

// Pomocnicze: czy nazwa z 8.3 pasuje do podanej nazwy (ignoruje wielkość)
static int fat32_name_match(const char* filename, const char* fat_name) {
    // filename np. "file.txt", fat_name to 11 bajtów w stylu 8.3 bez kropki
    char name[9] = { ' ' };     // 8 znaków + NULL
    char ext[4] = { ' ' };      // 3 znaki + NULL

    const char* dot = strrchr(filename, '.');
    size_t name_len = dot ? (size_t)(dot - filename) : strlen(filename);
    size_t ext_len = dot ? strlen(dot + 1) : 0;

    if (name_len > 8) name_len = 8;
    if (ext_len > 3) ext_len = 3;

    // kopiuj nazwę i rozszerzenie, uzupełnij spacjami
    memset(name, ' ', 8);
    for (size_t i = 0; i < name_len; i++)
        name[i] = toupper((unsigned char)filename[i]);

    memset(ext, ' ', 3);
    for (size_t i = 0; i < ext_len; i++)
        ext[i] = toupper((unsigned char)dot ? dot[1 + i] : '\0');

    // porównaj do fat_name
    return (memcmp(fat_name, name, 8) == 0) && (memcmp(fat_name + 8, ext, 3) == 0);
}

// Wylicz sektor pierwszego klastra
static uint32_t fat32_cluster_to_sector(uint32_t cluster) {
    return fat32_data_start_sector + (cluster - 2) * fat32_bpb.sectors_per_cluster;
}

// Przejdź do następnego klastra łańcucha FAT
uint32_t fat32_next_cluster(uint32_t current_cluster) {
    uint8_t fat_sector_buffer[512];
    uint32_t fat_offset = current_cluster * 4;
    uint32_t fat_sector = fat32_fat_start_sector + (fat_offset / fat32_bpb.bytes_per_sector);
    uint32_t ent_offset = fat_offset % fat32_bpb.bytes_per_sector;

    if (fat32_read_sector(fat_sector, fat_sector_buffer) != 0)
        return FAT32_CLUSTER_BAD;

    uint32_t next_cluster = *(uint32_t*)&fat_sector_buffer[ent_offset] & 0x0FFFFFFF; // 28 bitów

    if (next_cluster >= FAT32_CLUSTER_END)
        return 0; // koniec łańcucha

    return next_cluster;
}


int fat32_list_dir(uint32_t cluster) {
    if (cluster == 0)
        cluster = fat32_bpb.root_cluster;

    uint8_t sector[512];
    uint32_t sector_index = 0;
    uint32_t sectors_per_cluster = fat32_bpb.sectors_per_cluster;
    uint32_t current_cluster = cluster;

    while (current_cluster != 0) {
        uint32_t first_sector = fat32_cluster_to_sector(current_cluster);

        for (sector_index = 0; sector_index < sectors_per_cluster; sector_index++) {
            if (fat32_read_sector(first_sector + sector_index, sector) != 0)
                return -1;

            for (int i = 0; i < 512; i += sizeof(FAT32_DirEntry_t)) {
                FAT32_DirEntry_t* entry = (FAT32_DirEntry_t*)&sector[i];
                if (entry->name[0] == FAT32_ENTRY_FREE) 
                    return 0; // koniec katalogu
                if (entry->name[0] == FAT32_ENTRY_DELETED) 
                    continue;
                if (entry->attr == FAT32_ATTR_VOLUME_ID) 
                    continue;

                // Tu można wypisać lub zinterpretować entry - ale według Twojego założenia: tylko zwróć kod
                // Można też wywołać callback albo zapisać gdzieś, ale tu 0 oznacza sukces
            }
        }

        current_cluster = fat32_next_cluster(current_cluster);
    }

    return 0;
}

int fat32_find(const char* filename, FAT32_DirEntry_t* out_entry) {
    uint32_t cluster = fat32_bpb.root_cluster;
    uint8_t sector[512];
    uint32_t sectors_per_cluster = fat32_bpb.sectors_per_cluster;

    while (cluster != 0) {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < sectors_per_cluster; s++) {
            if (fat32_read_sector(first_sector + s, sector) != 0)
                return -1;

            for (int i = 0; i < 512; i += sizeof(FAT32_DirEntry_t)) {
                FAT32_DirEntry_t* entry = (FAT32_DirEntry_t*)&sector[i];

                if (entry->name[0] == FAT32_ENTRY_FREE) 
                    return -2; // Nie znaleziono

                if (entry->name[0] == FAT32_ENTRY_DELETED) 
                    continue;

                if (entry->attr == FAT32_ATTR_VOLUME_ID)
                    continue;

                if (fat32_name_match(filename, entry->name)) {
                    memcpy(out_entry, entry, sizeof(FAT32_DirEntry_t));
                    return 0; // znaleziono
                }
            }
        }

        cluster = fat32_next_cluster(cluster);
    }

    return -2; // Nie znaleziono
}

int fat32_read_file(const char* filename, uint8_t* buffer, size_t max_size) {
    FAT32_DirEntry_t entry;
    if (fat32_find(filename, &entry) != 0)
        return -1; // Nie znaleziono pliku

    uint32_t cluster = (entry.first_cluster_high << 16) | entry.first_cluster_low;
    size_t remaining = entry.file_size;
    if (remaining > max_size) remaining = max_size;

    size_t bytes_read = 0;
    uint8_t sector[512];

    while (cluster != 0 && remaining > 0) {
        uint32_t first_sector = fat32_cluster_to_sector(cluster);

        for (uint32_t s = 0; s < fat32_bpb.sectors_per_cluster && remaining > 0; s++) {
            if (fat32_read_sector(first_sector + s, sector) != 0)
                return -2; // Błąd odczytu

            size_t to_copy = remaining > 512 ? 512 : remaining;
            memcpy(buffer + bytes_read, sector, to_copy);

            bytes_read += to_copy;
            remaining -= to_copy;
        }

        cluster = fat32_next_cluster(cluster);
    }

    return (int)bytes_read;
}
