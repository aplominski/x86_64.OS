#include "keyboard.h"
#include <stdbool.h>
#include <stdint.h>


// --- Port I/O ---
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0"
                      : "=a"(ret)
                      : "Nd"(port));
    return ret;
}

// --- Mapowanie scancode → ASCII ---

static bool shift_pressed = false;
static bool extended = false;
static bool caps_lock = false;

static const char normal_map[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=','\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,    // Control
    'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,    // Left shift
    '\\','z','x','c','v','b','n','m',',','.','/',
    0,    // Right shift
    '*',
    0,    // Alt
    ' ',  // Space
};

static const char shift_map[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',
    0,
    'A','S','D','F','G','H','J','K','L',':','"','~',
    0,
    '|','Z','X','C','V','B','N','M','<','>','?',
    0,
    '*',
    0,
    ' ',
};

static char scancode_to_ascii(uint8_t sc) {
    if (sc == 0x2A || sc == 0x36) { shift_pressed = true;  return 0; }
    if (sc == 0xAA || sc == 0xB6) { shift_pressed = false; return 0; }

    if (sc == 0x3A) { // Caps Lock
        caps_lock = !caps_lock;
        return 0;
    }

    if (sc & 0x80) return 0; // ignoruj release

    char c = shift_pressed ? shift_map[sc] : normal_map[sc];

    // Obsługa Caps Lock - zmiana małych liter na wielkie i odwrotnie jeśli shift jest wciśnięty
    if (caps_lock && c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
    } else if (caps_lock && shift_pressed && c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a';
    }

    return c;
}

// --- API ---

void keyboard_init(void) {
    shift_pressed = false;
    extended = false;
}

KeyEvent get_key(void) {
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);

            if (sc == 0xE0) {
                extended = true;
                continue;
            }

            if (extended) {
                extended = false;
                switch (sc) {
                    case 0x48: return (KeyEvent){KEY_ARROW_UP, 0};
                    case 0x50: return (KeyEvent){KEY_ARROW_DOWN, 0};
                    case 0x4B: return (KeyEvent){KEY_ARROW_LEFT, 0};
                    case 0x4D: return (KeyEvent){KEY_ARROW_RIGHT, 0};
                    default: continue; // ignoruj inne extended
                }
            }

            char c = scancode_to_ascii(sc);
            if (c) {
                return (KeyEvent){KEY_CHAR, c};
            }
        }
    }
}
