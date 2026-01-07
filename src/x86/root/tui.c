#include <stdint.h>
#include <stdbool.h>
#include "../driver/keyboard.h"
#include "../sh/shell.h"  // shell_main, arc_reboot
//#include "../root/kernel.h" // not needed
extern void arc_reboot();

// arrow key defines (from keyboard driver)
#define ARROW_UP     1
#define ARROW_DOWN   2
#define ARROW_LEFT   3
#define ARROW_RIGHT  4

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#define COLOR_SEL  0x0F
#define COLOR_DIM  0x07

static volatile uint8_t* const VGA_MEMORY = (volatile uint8_t*)0xB8000;

// ===== MENU =====
static const char* menu_items[3] = {
    "Code Editor",
    "Shell",
    "Reboot"
};

static int selected = 0;
static bool running = true;

// ===== DRAW =====
static void draw_text(int x, int y, const char* s, uint8_t color) {
    int pos = y * VGA_WIDTH + x;
    while (*s) {
        VGA_MEMORY[pos*2] = *s++;
        VGA_MEMORY[pos*2+1] = color;
        pos++;
    }
}

static void draw_ui(void) {
    clear_screen();  // use the kernel/shell version

    draw_text(35, 4, "ArcOS - Apps", COLOR_SEL);

    for (int i = 0; i < 3; i++) {
        int y = 10 + i;
        if (i == selected) {
            draw_text(30, y, "| ", COLOR_SEL);
            draw_text(32, y, menu_items[i], COLOR_SEL);
        } else {
            draw_text(30, y, "  ", COLOR_DIM);
            draw_text(32, y, menu_items[i], COLOR_DIM);
        }
    }

    draw_text(18, 20, "Arrow Up/Down: Navigate | Enter: Select", COLOR_DIM);
}

// ===== INPUT =====
static void handle_key(char c) {
    switch(c) {
        case ARROW_UP:
            if (selected > 0) selected--;
            break;
        case ARROW_DOWN:
            if (selected < 2) selected++;
            break;
        case '\n':
        case '\r':
            running = false;
            break;
        default:
            break;
    }
}

// ===== PUBLIC ENTRY =====
void ui_start(void) {
    running = true;
    selected = 0;

    while (running) {
        draw_ui();
        char c = keyboard_getc();
        handle_key(c);
    }

    clear_screen();

    // ===== ACTIONS =====
    switch (selected) {
        case 0: // Code Editor
            draw_text(28, 12, "Code Editor not implemented", COLOR_DIM);
            while(1) {} // halt
            break;
        case 1: // Shell
            shell_main();
            break;
        case 2: // Reboot
            arc_reboot();
            break;
    }
}
