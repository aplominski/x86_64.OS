#include "Disks.h"
#include "../heap/heap.h"
#include <stddef.h>

extern storage_device_t* kspace_actual_storage_device = NULL;

void select_device(storage_device_type_t type, int device_address_a, int device_address_b) {
    if (kspace_actual_storage_device == NULL) {
        kspace_actual_storage_device = (storage_device_t*)malloc(sizeof(storage_device_t));
    }
    
    kspace_actual_storage_device->type = type;
    kspace_actual_storage_device->device_address_a = device_address_a;
    kspace_actual_storage_device->device_address_b = device_address_b;
}