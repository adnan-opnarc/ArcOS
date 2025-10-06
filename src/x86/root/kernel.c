__attribute__((section(".multiboot"), used)) static const unsigned int multiboot_header[] = {
    0x1BADB002, 0x00, -(0x1BADB002 + 0x00)
};

#include <stdint.h>
#include <stdbool.h>
#include "../crp/io.h"
#include "../sh/shell.h"
#include "../driver/keyboard.h"
#include "../uilib/ui.h"
#include "kernel_panic.h"

void shell_main(void);

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define WHITE_ON_BLACK 0x0F
static volatile uint8_t* const VGA_MEMORY = (volatile uint8_t*)0xB8000;

static int cursor_pos = 0;

void update_cursor() {
    uint16_t pos = cursor_pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scroll_screen() {
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < VGA_WIDTH; col++) {
            int from = (row * VGA_WIDTH + col) * 2;
            int to = ((row - 1) * VGA_WIDTH + col) * 2;
            VGA_MEMORY[to] = VGA_MEMORY[from];
            VGA_MEMORY[to + 1] = VGA_MEMORY[from + 1];
        }
    }

    for (int col = 0; col < VGA_WIDTH; col++) {
        int idx = ((VGA_HEIGHT - 1) * VGA_WIDTH + col) * 2;
        VGA_MEMORY[idx] = ' ';
        VGA_MEMORY[idx + 1] = WHITE_ON_BLACK;
    }

    cursor_pos = VGA_WIDTH * (VGA_HEIGHT - 1);
}

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

void printk(const char* str) {
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

void delay() {
    for (volatile unsigned long i = 0; i < 40000000; i++) {}
}

void stdelay() {
    for (volatile unsigned long i = 0; i < 10000000; i++) {}
}

void dbfs() {
    for (volatile unsigned long i = 0; i < 200000000UL; i++) {}
}

void kernel_main(void);

void show_splash(void) {
    clear_screen();
    printk("       /\\        \n");
    printk("      /  \\       \n");
    printk("     / /\\ \\      \n");
    printk("    / ____ \\     \n");
    printk("   /_/    \\_\\    \n");
    printk("\n     Arc OS      \n");
    dbfs();
}

void kernelbfs(void) {
    clear_screen();
    printk("[0.120] boot sequence ended\n");
    delay();
    printk("[1.200] shifting to core language\n");
    delay();
    printk("[1.300] loading core modules\n");
    delay();
    printk("[1.400] starting core services\n");
    delay();
    printk("[1.500] loaded shell.o\n");
    delay();
    printk("[1.600] shifting to shell in few second\n");
    delay();
    printk("[000] session has started may be theres a issue in kernel\n");
    delay();
    clear_screen();
    printk("[#------------------------]");
    stdelay();
    clear_screen();
    printk("[##----------------------]");
    stdelay();
    clear_screen();
    printk("[####--------------------]");
    stdelay();
    clear_screen();
    printk("[########----------------]");
    stdelay();
    clear_screen();
    printk("[############------------]");
    stdelay();
    clear_screen();
    printk("[################--------]");
    stdelay();
    clear_screen();
    printk("[####################----]");
    stdelay();
    clear_screen();
    printk("[########################]");
    delay();
    clear_screen();
}

void kernel_main(void) {
    show_splash();
    clear_screen();
    //kernel_panic(); //is on work dont touch
    kernelbfs();
    printk("Welcome to ArcOS! \n");
    printk("Log in as root user.\n");
    shell_main();

    while (1) {}
}

