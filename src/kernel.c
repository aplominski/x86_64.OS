#include "VGA/vga.h"
#include "VGA/vga_tty_resolution.h"
#include "keyboard.h"
#include "console.h"

#include "PCI/pci_device.h"
#include "PCI/pci_class_explain.h"  
#include "Disks/IDE/ide.h"
#include "heap/heap.h"
#include "Disks/Disks.h"
#include "Disks/FAT32/fat32.h"

#include "x86_64/memory/max_memory.h"
int test1() {
    
}

void print_pci_device(const PCIDevice_t* dev) {
    char tmp[8];

    puts("PCI Device: ", false);
    print_hex16(dev->vendor_id);
    putchar(' ', false);
    print_hex16(dev->device_id);


    putchar('\n', false);
}

extern void uitoa(unsigned num, char* out);

void kmain() {


    clear_vga_output();
    heap_init();
    keyboard_init();

    set_vga_color(VGA_COLOR_YELLOW, VGA_COLOR_GREEN);
    set_vga_bg_color(VGA_COLOR_CYAN);
    set_vga_fg_color(VGA_COLOR_BLACK);

    uint32_t ram_bytes = get_max_memory();
    puts("RAM size: ",false);
    print_dec(ram_bytes / (1024 * 1024));
    puts(" MiB\n",false);



    puts("Kernel sie uruchamia. \n",false);

    PCIDevice_t devices[32];
    size_t count = pcie_scan_bus(devices, 32);
    print_number(count);

    for (size_t i = 0; i < pcie_get_device_count(); ++i) {
        print_pci_device(&devices[i]);
    }
    

    IDEDevice_t* ide = ide_detect_all();

    for(int i = 0; i < 4; ++i) {
        IDEDevice_t dev = ide[i];
        puts("IDE Device: ", false);
        puts(dev.type, false);
        putchar(' ', false);
        puts(dev.model, false);
        putchar('\n', false);
    }

    putchar('\n', false);
    select_device(STORAGE_IDE, 0, 0);
    print_number(test1());

    // while (1) {
    //     KeyEvent k = get_key();
    //     switch (k.type) {
    //         case KEY_CHAR:
    //             putchar(k.ch,true);
    //             break;
    //         case KEY_ARROW_UP:
    //             move_cursor(0, -1);
    //             break;
    //         case KEY_ARROW_DOWN:
    //             move_cursor(0, 1);
    //             break;
    //         case KEY_ARROW_LEFT:
    //             move_cursor(-1, 0);
    //             break;
    //         case KEY_ARROW_RIGHT:
    //             move_cursor(1, 0);
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    
}