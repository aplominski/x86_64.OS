#ifndef __DISKS_H
#define __DISKS_H

typedef enum {
    STORAGE_IDE,
    STORAGE_RAMFS
} storage_device_type_t;

typedef struct {
    storage_device_type_t type;
    int device_address_a; // IN IDE: Channel
    int device_address_b; // IN IDE: Drive
} storage_device_t;

extern storage_device_t* kspace_actual_storage_device;

void select_device(storage_device_type_t type, int device_address_a, int device_address_b);

#endif