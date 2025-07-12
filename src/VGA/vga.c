#include <stdint.h>
#include <stddef.h>

#include "vga.h"

uint16_t* vga_buffer = (uint16_t*)VGA_ADDR;
uint8_t vga_row = 0;
uint8_t vga_col = 0;
uint8_t default_color = 0x01; // jasnoszary na czarnym tle

void clear_vga_output() {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = ((uint16_t)default_color << 8) | ' ';
    }
    vga_row = 0;
    vga_col = 0;
}


vga_color_t get_vga_fg_color(void) {
    return default_color & 0x0F; // zwraca kolor czcionki
}
vga_color_t get_vga_bg_color(void) {
    return (default_color >> 4) & 0x0F; // zwraca kolor tła
}

void set_vga_color(vga_color_t fg, vga_color_t bg) {
    default_color = (bg << 4) | (fg & 0x0F);
}
void set_vga_fg_color(vga_color_t color) {
    default_color = (default_color & 0xF0) | (color & 0x0F);
}
void set_vga_bg_color(vga_color_t color) {
    default_color = (default_color & 0x0F) | ((color & 0x0F) << 4);
}