#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"
#include "../crp/io.h"

// US QWERTY scancode set 1 to ASCII (with shift)
static const char scancode_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x','c','v','b','n','m',',','.','/', 0,
    '*', 0,' ', 0,
    // rest zeros
};

static const char scancode_ascii_shift[128] = {
    0,  27, '!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0,'|','Z','X','C','V','B','N','M','<','>','?', 0,
    '*', 0,' ', 0,
    // rest zeros
};

static bool shift = false;
static bool caps = false;

#define ARROW_UP     1
#define ARROW_DOWN   2
#define ARROW_LEFT   3
#define ARROW_RIGHT  4

void keyboard_init(void) {
    shift = false;
    caps = false;
}

char keyboard_getc(void) {
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t sc = inb(0x60);

            // Handle extended keys (like arrows)
            if (sc == 0xE0) {
                uint8_t ext = inb(0x60);
                switch (ext) {
                    case 0x48: return ARROW_UP;
                    case 0x50: return ARROW_DOWN;
                    case 0x4B: return ARROW_LEFT;
                    case 0x4D: return ARROW_RIGHT;
                    default: break;
                }
                continue;
            }

            // Key release
            if (sc & 0x80) {
                sc &= 0x7F;
                if (sc == 42 || sc == 54) shift = false; // Shift release
            } else {
                if (sc == 42 || sc == 54) { shift = true; continue; } // Shift press
                if (sc == 58) { caps = !caps; continue; }             // Caps lock

                char c = 0;
                if (sc < 128) {
                    char normal = scancode_ascii[sc];
                    char shifted = scancode_ascii_shift[sc];

                    if (normal >= 'a' && normal <= 'z') {
                        c = (shift ^ caps) ? shifted : normal;
                    } else {
                        c = shift ? shifted : normal;
                    }
                }

                if (c) return c;
            }
        }
    }
}

