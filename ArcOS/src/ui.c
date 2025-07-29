#include "ui.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F
static volatile unsigned char* const VGA = (unsigned char*)0xB8000;

static void vga_write_char(int x, int y, char c, unsigned char attr) {
    int index = (y * VGA_WIDTH + x) * 2;
    VGA[index] = c;
    VGA[index + 1] = attr;
}

void ui_draw_box(int x, int y, int w, int h, const char* title) {
    // Top
    vga_write_char(x, y, '+', WHITE_ON_BLACK);
    for (int i = 1; i < w - 1; i++) vga_write_char(x + i, y, '-', WHITE_ON_BLACK);
    vga_write_char(x + w - 1, y, '+', WHITE_ON_BLACK);

    // Sides
    for (int j = 1; j < h - 1; j++) {
        vga_write_char(x, y + j, '|', WHITE_ON_BLACK);
        vga_write_char(x + w - 1, y + j, '|', WHITE_ON_BLACK);
    }

    // Bottom
    vga_write_char(x, y + h - 1, '+', WHITE_ON_BLACK);
    for (int i = 1; i < w - 1; i++) vga_write_char(x + i, y + h - 1, '-', WHITE_ON_BLACK);
    vga_write_char(x + w - 1, y + h - 1, '+', WHITE_ON_BLACK);

    // Title
    if (title) {
        int len = 0;
        while (title[len]) len++;
        for (int i = 0; i < len && i + x + 2 < x + w - 2; i++) {
            vga_write_char(x + 2 + i, y, title[i], WHITE_ON_BLACK);
        }
    }
}

void ui_draw_label(int x, int y, const char* text) {
    int i = 0;
    while (text[i]) {
        vga_write_char(x + i, y, text[i], WHITE_ON_BLACK);
        i++;
    }
}

void ui_draw_header_bar(const char* title) {
    for (int i = 0; i < VGA_WIDTH; i++) {
        vga_write_char(i, 0, '=', WHITE_ON_BLACK);
    }

    int i = 0;
    while (title[i] && i + 2 < VGA_WIDTH) {
        vga_write_char(2 + i, 0, title[i], WHITE_ON_BLACK);
        i++;
    }
}

