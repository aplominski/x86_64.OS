// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>


typedef enum {
    KEY_NONE = 0,
    KEY_CHAR,       // zwykły znak ASCII
    KEY_ARROW_UP,
    KEY_ARROW_DOWN,
    KEY_ARROW_LEFT,
    KEY_ARROW_RIGHT,
} KeyType;

typedef struct {
    KeyType type;
    char ch;       // ważne tylko jeśli type == KEY_CHAR
} KeyEvent;

// Zainicjalizuj stan klawiatury (tylko reset flag Shift)
void keyboard_init(void);

// Blokujący odczyt znaku ASCII (polling)
KeyEvent get_key(void);

#endif // KEYBOARD_H
