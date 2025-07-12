CC = gcc
AS = nasm
KERNEL = build_dir/kernel.bin
BOOT_DIR = boot
CC_FLAGS = -ffreestanding -mno-red-zone -m32 -nostdlib -O2 -Wall -Wextra 
BUILD_DIR = build_dir

OBJECTS = \
	$(BUILD_DIR)/kernel.o \
	$(BUILD_DIR)/kernel.asm.o \
	$(BUILD_DIR)/max_memory.o \
	$(BUILD_DIR)/keyboard.o \
	$(BUILD_DIR)/vga.o \
	$(BUILD_DIR)/vga_tty_resolution.o \
	$(BUILD_DIR)/console.o \
	$(BUILD_DIR)/pci_device.o \
	$(BUILD_DIR)/pci_read.o \
	$(BUILD_DIR)/pci_utils.o \
	$(BUILD_DIR)/heap_malloc.o \
	$(BUILD_DIR)/heap_calloc.o \
	$(BUILD_DIR)/heap_realloc.o \
	$(BUILD_DIR)/heap_free.o \
	$(BUILD_DIR)/disks.o \
	$(BUILD_DIR)/ide_utils.o \
	$(BUILD_DIR)/ide_detect.o \
	$(BUILD_DIR)/fat32_low.o \
	$(BUILD_DIR)/fat32_public.o \
	$(BUILD_DIR)/ramfs_init.o \
	$(BUILD_DIR)/ramfs_destroy.o \
	$(BUILD_DIR)/ramfs_create.o \
	$(BUILD_DIR)/ramfs_remove.o \
	$(BUILD_DIR)/ramfs_getters.o \
	$(BUILD_DIR)/ramfs_read.o \
	$(BUILD_DIR)/ramfs_save.o \
	$(BUILD_DIR)/string.o 


all: build_dir $(KERNEL) copy iso

$(KERNEL): $(OBJECTS)
	ld -m elf_i386 -nostdlib -T linker.ld -o $@ $(OBJECTS)

build_dir:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BOOT_DIR)

$(BUILD_DIR)/kernel.o: src/kernel.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/kernel.asm.o: src/x86_64/kernel.asm
	$(AS) -f elf32 $< -o $@

$(BUILD_DIR)/max_memory.o: src/x86_64/memory/max_memory.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/keyboard.o: src/keyboard.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: src/VGA/vga.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/vga_tty_resolution.o: src/VGA/vga_tty_resolution.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/console.o: src/console.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/pci_device.o: src/PCI/pci_device.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/pci_read.o: src/PCI/pci_read.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/pci_utils.o: src/PCI/pci_utils.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/heap_malloc.o: src/heap/malloc.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/heap_calloc.o: src/heap/calloc.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/heap_realloc.o: src/heap/realloc.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/heap_free.o: src/heap/free.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/disks.o: src/Disks/Disks.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ide_utils.o: src/Disks/IDE/ide_utils.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ide_detect.o: src/Disks/IDE/ide_detect.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/fat32_low.o: src/Disks/FAT32/fat32_low.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/fat32_public.o: src/Disks/FAT32/fat32_public.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_init.o: src/Disks/ramfs/ramfs_init.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_destroy.o: src/Disks/ramfs/ramfs_destroy.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_create.o: src/Disks/ramfs/ramfs_create.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_remove.o: src/Disks/ramfs/ramfs_remove.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_getters.o: src/Disks/ramfs/ramfs_getters.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_read.o: src/Disks/ramfs/ramfs_read.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/ramfs_save.o: src/Disks/ramfs/ramfs_save.c
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)/string.o: src/utils/string.c
	$(CC) $(CC_FLAGS) -c $< -o $@

iso: $(KERNEL)
	grub2-mkrescue -o panix.iso $(BOOT_DIR)/

copy:
	cp $(KERNEL) $(BOOT_DIR)/

qemu:
	qemu-system-x86_64 -m 2 -cdrom panix.iso -boot d -drive file=disk.img,format=raw,if=ide,index=0

clean:
	rm -rf $(BUILD_DIR) $(KERNEL) panix.iso $(BOOT_DIR)/kernel.bin