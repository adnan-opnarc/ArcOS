#include <stdint.h>
#include <stdbool.h>
#include "../crp/io.h"

// Arrow key values for shell/tui
#define ARROW_UP     1
#define ARROW_DOWN   2
#define ARROW_LEFT   3
#define ARROW_RIGHT  4

static const char scancode_ascii[128] = {
    0, 27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\','z','x','c','v','b','n','m',',','.','/',0,
    '*',0,' ',0
};

static const char scancode_ascii_shift[128] = {
    0,27,'!','@','#','$','%','^','&','*','(',')','_','+','\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,
    'A','S','D','F','G','H','J','K','L',':','"','~',0,'|','Z','X','C','V','B','N','M','<','>','?',0,
    '*',0,' ',0
};

static bool shift = false;
static bool caps  = false;

char keyboard_getc(void) {
    static bool extended = false;

    while (1) {
        if (!(inb(0x64) & 1)) continue; // wait for data

        uint8_t sc = inb(0x60);

        // extended key prefix
        if (sc == 0xE0) {
            extended = true;
            continue;
        }

        bool released = sc & 0x80;
        sc &= 0x7F; // remove high bit

        // Shift keys
        if (sc == 42 || sc == 54) { // shift
            shift = !released;
            continue;
        }

        // Caps Lock
        if (sc == 58 && !released) {
            caps = !caps;
            continue;
        }

        // arrow keys (only on press, not release)
        if (extended && !released) {
            extended = false;
            switch (sc) {
                case 72: return ARROW_UP;
                case 80: return ARROW_DOWN;
                case 75: return ARROW_LEFT;
                case 77: return ARROW_RIGHT;
            }
        }

        extended = false;

        // normal keys
        if (!released && sc < 128) {
            char normal = scancode_ascii[sc];
            char shifted = scancode_ascii_shift[sc];

            if (normal >= 'a' && normal <= 'z')
                return (shift ^ caps) ? shifted : normal;
            else
                return shift ? shifted : normal;
        }
    }
}
