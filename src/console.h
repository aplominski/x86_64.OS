#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void putchar(char c,bool removable);
void puts(const char* str, bool removable);
void print_uint64(uint64_t value);
void print_dec(unsigned int value);
void print_number(size_t n);
void print_hex16(uint16_t val);
void print_hex8(uint8_t val);
void move_cursor(int dx, int dy);

#endif