// Arc os by Adnan opnarc  Licensed under Mit


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
#include "font.h"
#include "../crp/bitmap.h"
#include "../sh/bitmap.h"
#include "../fs/ramfs.h"
#include "tui.h"

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

// Boot animation bitmap (16x16)
static const uint8_t boot_bitmap[] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x7f,0xff,0xc7,0x9f,0xff,0xff,0xff,0xff,0xff,0xff,0x3f,0xff,0x03,0x0f,0xff,0xff,0xff,0xff,0xff,0xfe,0x38,0x23,0x38,0x6f,0xff,0xff,0xff,0xff,0xff,0xfe,0x18,0x00,0x79,0x3f,0xff,0xff,0xff,0xff,0xff,0xfe,0x99,0xc8,0x79,0x0f,0xff,0xff,0xff,0xff,0xff,0xfc,0x19,0xce,0x79,0xc7,0xff,0xff,0xff,0xff,0xff,0xfd,0xc9,0xcf,0x38,0x67,0xff,0xff,0xff,0xff,0xff,0xf9,0xc9,0xc1,0x12,0x67,0xff,0xff,0xff,0xff,0xff,0xf8,0x80,0xe1,0x87,0x0f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff};
// Render bitmap at position with block characters
static void render_boot_bitmap(int x, int y, const uint8_t* data, int width, int height) {
    bitmap_t bmp = bitmap_create(data, (width * height + 7) / 8, width, height, BITMAP_FORMAT_MONO);
    if (!bitmap_is_valid(&bmp)) return;
    
    for (int by = 0; by < height; by++) {
        move_cursor(x, y + by);
        for (int bx = 0; bx < width; bx++) {
            uint8_t pixel = bitmap_get_pixel_raw(&bmp, bx, by);
            uint8_t right = (bx + 1 < width) ? bitmap_get_pixel_raw(&bmp, bx + 1, by) : 0;
            uint8_t down = (by + 1 < height) ? bitmap_get_pixel_raw(&bmp, bx, by + 1) : 0;
            uint8_t diag = (bx + 1 < width && by + 1 < height) ? 
                          bitmap_get_pixel_raw(&bmp, bx + 1, by + 1) : 0;
            
            // Smooth blending with block characters
            if (pixel && right && down && diag) {
                putchar(0xDB);  // Full block
            } else if (pixel && right) {
                putchar(0xDC);  // Lower half
            } else if (pixel && down) {
                putchar(0xDF);  // Upper half
            } else if (pixel) {
                putchar(0xB0);  // Light shade
            } else if (right || down || diag) {
                putchar(0xB1);  // Medium shade
            } else {
                putchar(' ');   // Space
            }
        }
        // Move to next line after each row
        putchar('\n');
    }
}

void show_splash(void) {
    clear_screen();
    
    // Center the bitmap on screen (80x25, bitmap is 16x16)
    int center_x = (80 - 16) / 2;
    int center_y = (25 - 16) / 2;
    
    // Animated boot sequence with bitmap
    for (int frame = 0; frame < 3; frame++) {
        clear_screen();
        
        // Fade in effect - draw multiple times with delay
        for (int i = 0; i <= frame; i++) {
            render_boot_bitmap(center_x, center_y, boot_bitmap, 16, 16);
            stdelay();
        }
        
        // Show "ArcOS" text below
        move_cursor(center_x + 2, center_y + 18);
        printk("ArcOS");
        putchar('\n');  // Newline after text
        
        dbfs();
    }
    
    // Final display
    clear_screen();
    render_boot_bitmap(center_x, center_y, boot_bitmap, 16, 16);
    move_cursor(center_x + 2, center_y + 18);
    printk("ArcOS");
    putchar('\n');  // Newline after text
    dbfs();
}

void kernelbfs(void) {
    clear_screen();
    
    int center_x = (80 - 16) / 2;
    int center_y = (25 - 16) / 2;
    
    // Animated boot sequence with bitmap
    // Show bitmap with loading animation
    for (int step = 0; step < 8; step++) {
        clear_screen();
        
        // Draw bitmap
        render_boot_bitmap(center_x, center_y, boot_bitmap, 16, 16);
        
        // Show loading progress below
        move_cursor(center_x - 5, center_y + 18);
        printk("[");
        for (int i = 0; i < step; i++) {
            putchar('#');
        }
        for (int i = step; i < 20; i++) {
            putchar('-');
        }
        printk("]");
        putchar('\n');  // Newline after progress bar
        
        // Show status text
        move_cursor(center_x - 5, center_y + 19);
        switch (step) {
            case 0: printk("Loading kernel..."); break;
            case 1: printk("Initializing modules..."); break;
            case 2: printk("Starting services..."); break;
            case 3: printk("Loading drivers..."); break;
            case 4: printk("Mounting filesystems..."); break;
            case 5: printk("Starting shell..."); break;
            case 6: printk("Almost ready..."); break;
            case 7: printk("Boot complete!"); break;
        }
        putchar('\n');  // Ensure newline after status
        
        stdelay();
        delay();
    }
    
    // Final clear before shell
    clear_screen();
}

void init_font(void);

void ram_addr(void){
    printk("0x00000, ");
    stdelay();
    printk("0x00001, ");
    stdelay();
    printk("0x11111, ");
    dbfs();
    clear_screen();
    printk("kernel proc 00001\n");
    printk("kernel nprs x09::1\n");
    dbfs();
    dbfs();
}

void kernel_main(void) {
    clear_screen();
    ram_addr();
	init_font();
    //show_splash();
    //clear_screen();
    //kernel_panic(); //is on work dont touch
    // Initialize filesystem
    ramfs_init();

    printk("Welcome to ArcOS! \n");
    printk("Log in as root user.\n");
    //shell_main();
    ui_start();
    while (1) {}
}

