// Multiboot1 header
__attribute__((section(".multiboot"), used)) static const unsigned int multiboot_header[] = {
    0x1BADB002, 0x00, -(0x1BADB002 + 0x00)
};

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "io.h"
#include "shell.h"
#include "keyboard.h"

// Forward declaration for shell
void shell_main(void);

// --- VGA Constants ---
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F
static volatile uint8_t* const VGA_MEMORY = (volatile uint8_t*)0xB8000;

static int cursor_pos = 0;

// --- Cursor blinking using VGA ports ---
void update_cursor() {
    uint16_t pos = cursor_pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// --- Screen scrolling ---
static void scroll_screen() {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int from = (row * VGA_WIDTH + col) * 2;
            int to = ((row - 1) * VGA_WIDTH + col) * 2;
            VGA_MEMORY[to] = VGA_MEMORY[from];
            VGA_MEMORY[to + 1] = VGA_MEMORY[from + 1];
        }
    }
    // Clear the last row
    for (int col = 0; col < VGA_WIDTH; col++) {
        int idx = ((VGA_HEIGHT - 1) * VGA_WIDTH + col) * 2;
        VGA_MEMORY[idx] = ' ';
        VGA_MEMORY[idx + 1] = WHITE_ON_BLACK;
    }
    cursor_pos = VGA_WIDTH * (VGA_HEIGHT - 1);
}

// --- Character output ---
void putchar(char c) {
    if (c == '\n') {
        cursor_pos = (cursor_pos / VGA_WIDTH + 1) * VGA_WIDTH;
    } else if (c == '\b') {
        if (cursor_pos > 0) {
            cursor_pos--;
            int idx = cursor_pos * 2;
            VGA_MEMORY[idx] = ' ';
            VGA_MEMORY[idx + 1] = WHITE_ON_BLACK;
        }
    } else {
        int idx = cursor_pos * 2;
        VGA_MEMORY[idx] = c;
        VGA_MEMORY[idx + 1] = WHITE_ON_BLACK;
        cursor_pos++;
    }

    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        scroll_screen();
    }

    update_cursor();
}

void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void clear_screen() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i * 2] = ' ';
        VGA_MEMORY[i * 2 + 1] = WHITE_ON_BLACK;
    }
    cursor_pos = 0;
    update_cursor();
}

// --- Delay ---
void delay() {
    for (volatile unsigned long i = 0; i < 10000000; i++) {}
}
void dbfs() {
    for (volatile unsigned long i = 0; i < 300000000UL; i++) {}
}

// --- Fake scan ---
void msc_main(void) {
    puts("\n[!] Running system scan...\n");
    delay();
    puts("[*] Checking filesystem...\n");
    delay();
    puts("[*] Checking memory...\n");
    delay();
    puts("[*] Analyzing processes...\n");
    delay();
    puts("[+] No issues found. System is clean.\n\n");
    delay();
}

// --- Splash ---
void show_splash(void) {
    msc_main();
    clear_screen();
    puts("       /\\        \n");
    puts("      /  \\       \n");
    puts("     / /\\ \\      \n");
    puts("    / ____ \\     \n");
    puts("   /_/    \\_\\    \n");
    puts("\n     Arc OS      \n");
    dbfs();
}

// --- Entry ---
void kernel_main(void) {
    show_splash();
    clear_screen();
    puts("Welcome to ArcOS!\n");
    puts("This version of ArcOS is named salvador x86. For commands type 'help' to get all commands listed.\n\n");
    delay();
    shell_main();
    while (1) {}
}

