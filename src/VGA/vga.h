#ifndef __VGA_H_
#define __VGA_H_
#define VGA_ADDR 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

#include <stdint.h>

typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_YELLOW = 14,
    VGA_COLOR_WHITE = 15
} vga_color_t;


extern uint16_t* vga_buffer;
extern uint8_t vga_row;
extern uint8_t vga_col;
extern uint8_t default_color;

void clear_vga_output();
void set_vga_fg_color(vga_color_t color); //set font color
void set_vga_bg_color(vga_color_t color); //set background color

vga_color_t get_vga_fg_color(void);
vga_color_t get_vga_bg_color(void);

void set_vga_color(vga_color_t fg, vga_color_t bg);
#endif