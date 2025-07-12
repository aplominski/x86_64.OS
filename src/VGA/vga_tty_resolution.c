#include "vga_tty_resolution.h"

#define VGA_CRTC_ADDR 0x3D4
#define VGA_CRTC_DATA 0x3D5
#define VGA_SEQ_ADDR  0x3C4
#define VGA_SEQ_DATA  0x3C5

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile ("inb %1, %0" : "=a"(val) : "Nd"(port));
    return val;
}

static void vga_write_crtc(uint8_t index, uint8_t value) {
    outb(VGA_CRTC_ADDR, index);
    outb(VGA_CRTC_DATA, value);
}

static uint8_t vga_read_crtc(uint8_t index) {
    outb(VGA_CRTC_ADDR, index);
    return inb(VGA_CRTC_DATA);
}

static void vga_write_seq(uint8_t index, uint8_t value) {
    outb(VGA_SEQ_ADDR, index);
    outb(VGA_SEQ_DATA, value);
}

static uint8_t vga_read_seq(uint8_t index) {
    outb(VGA_SEQ_ADDR, index);
    return inb(VGA_SEQ_DATA);
}

static void set_standard_80x25() {
    // Wyłącz double word mode (bit 6 reg 0x09 Sequencer)
    uint8_t val = vga_read_seq(0x09);
    val &= ~(1 << 6);
    vga_write_seq(0x09, val);

    // Standardowe wartości rejestrów CRTC dla 80x25 (dostosuj jeśli chcesz)
    vga_write_crtc(0x01, 0x5F); // horizontal total
    vga_write_crtc(0x02, 0x4F); // horizontal displayed
    vga_write_crtc(0x06, 0x4B); // vertical total
    vga_write_crtc(0x07, 0x00); // overflow
    vga_write_crtc(0x09, 0x3E); // vertical displayed
    // Możesz dodać resztę rejestrów, by tryb był kompletny
}

static void set_mode_160x50() {
    // Włącz double word mode (bit 6 reg 0x09 Sequencer)
    uint8_t val = vga_read_seq(0x09);
    val |= (1 << 6);
    vga_write_seq(0x09, val);

    // Przykładowe ustawienia rejestrów CRTC (dostosuj do karty i monitora!)
    vga_write_crtc(0x01, 0x7F); // horizontal total (127)
    vga_write_crtc(0x02, 0x4F); // horizontal displayed
    vga_write_crtc(0x06, 0x3F); // vertical total
    vga_write_crtc(0x07, 0x00); // overflow
    vga_write_crtc(0x09, 0x4F); // vertical displayed
}

void vga_set_mode(vga_mode_t mode) {
    switch (mode) {
        case VGA_MODE_80x25:
            set_standard_80x25();
            break;
        case VGA_MODE_160x50:
            set_mode_160x50();
            break;
        default:
            // Domyślnie 80x25
            set_standard_80x25();
            break;
    }
}

void vga_set_custom_mode(int columns, int rows) {
    if (columns == 160 && rows == 50) {
        set_mode_160x50();
    } else {
        set_standard_80x25();
    }
}
