#ifndef VGA_MODE_H
#define VGA_MODE_H

#include <stdint.h>

typedef enum {
    VGA_MODE_80x25,
    VGA_MODE_160x50
} vga_mode_t;

void vga_set_mode(vga_mode_t mode);
void vga_set_custom_mode(int columns, int rows); // szkielet, możesz rozszerzyć

#endif // VGA_MODE_H
