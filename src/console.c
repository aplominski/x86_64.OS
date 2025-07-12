#include "console.h"
#include "VGA/vga.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define BUFFER_SIZE (VGA_WIDTH * VGA_HEIGHT)
static const char HEX_CHARS[] = "0123456789ABCDEF";

// Tablica flag removable na każdej pozycji ekranu
static bool removable_map[BUFFER_SIZE];

void putchar(char c, bool removable) {
    if (c == '\n') {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_HEIGHT) vga_row = VGA_HEIGHT - 1;
        return;
    }

    if (c == '\b') {
        // Backspace działa tylko jeśli poprzedni znak removable
        if (vga_col == 0 && vga_row == 0) return; // początek ekranu, nie ma nic do usunięcia

        // Cofnij kolumnę/wiersz
        if (vga_col > 0) {
            vga_col--;
        } else {
            vga_row--;
            vga_col = VGA_WIDTH - 1;
        }

        size_t index = vga_row * VGA_WIDTH + vga_col;

        if (removable_map[index]) {
            // Usuń znak tylko jeśli removable
            vga_buffer[index] = ((uint16_t)default_color << 8) | ' ';
            removable_map[index] = false;
        } else {
            // Nieusuwalny znak, cofnij kursor spowrotem (nie usuwaj)
            if (vga_col < VGA_WIDTH - 1) {
                vga_col++;
            } else {
                if (vga_row < VGA_HEIGHT - 1) {
                    vga_row++;
                    vga_col = 0;
                }
            }
        }
        return;
    }

    size_t index = vga_row * VGA_WIDTH + vga_col;
    vga_buffer[index] = ((uint16_t)default_color << 8) | c;
    removable_map[index] = removable;

    vga_col++;
    if (vga_col >= VGA_WIDTH) {
        vga_col = 0;
        vga_row++;
        if (vga_row >= VGA_HEIGHT) vga_row = VGA_HEIGHT - 1;
    }
}

void puts(const char* str, bool removable) {
    while (*str) putchar(*str++,removable);
}

void print_uint64(uint64_t value) {
    char buf[21]; // max 20 cyfr + null
    buf[20] = 0;
    int i = 19;

    if (value == 0) {
        puts("0",false);
        return;
    }

    while (value > 0 && i >= 0) {
        buf[i--] = '0' + (value % 10);
        value /= 10;
    }

    puts(&buf[i + 1],false);
}

void print_dec(unsigned int value) {
    char buffer[10]; // wystarczy na 32-bitową liczbę
    int i = 0;

    if (value == 0) {
        putchar('0',false);
        return;
    }

    while (value > 0 && i < 10) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    // Wypisz w odwrotnej kolejności
    while (--i >= 0) {
        putchar(buffer[i],false);
    }
}


void print_number(size_t n) {
    if (n == 0) {
        putchar('0', false);
        return;
    }
    char buf[20];
    int i = 0;
    while (n > 0) {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    for (int j = i - 1; j >= 0; --j) {
        putchar(buf[j], false);
    }
}

// Wypisanie 8-bitowego bajtu w hex (np. 0xAF)
void print_hex8(uint8_t val) {
    putchar(HEX_CHARS[(val >> 4) & 0xF], false);
    putchar(HEX_CHARS[val & 0xF], false);
}

void print_hex16(uint16_t val) {
    print_hex8((val >> 8) & 0xFF);
    print_hex8(val & 0xFF);
}


void move_cursor(int dx, int dy) {
    int new_col = vga_col + dx;
    int new_row = vga_row + dy;

    // Ograniczenia w poziomie
    if (new_col < 0) {
        if (new_row > 0) {
            new_row--;
            new_col = VGA_WIDTH - 1;
        } else {
            new_col = 0;
        }
    } else if (new_col >= VGA_WIDTH) {
        new_col = 0;
        new_row++;
    }

    // Ograniczenia w pionie
    if (new_row < 0) new_row = 0;
    if (new_row >= VGA_HEIGHT) new_row = VGA_HEIGHT - 1;

    vga_col = new_col;
    vga_row = new_row;
}
