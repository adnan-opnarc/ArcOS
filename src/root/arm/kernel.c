#include <stdint.h>
#include <stdbool.h>
#include "../../driver/arm/uart.h"      // <- your UART implementation
#include "../../sh/arm/shell.h"         // <- your shell
#include "../../driver/arm/keyboard.h"  // optional
#include "../../uilib/arm/ui.h"         // optional

void shell_main(void);

void putchar(char c) {
    if (c == '\n') {
        uart_send('\r'); // for proper terminal newlines
    }
    uart_send(c);
}

void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

void delay() {
    for (volatile unsigned long i = 0; i < 10000000; i++) {}
}

void dbfs() {
    for (volatile unsigned long i = 0; i < 200000000UL; i++) {}
}

void msc_main(void) {
    puts("\n[!] Running system scan...\n");
    delay();
    puts("[*] Checking filesystem...\n");
    delay();
    puts("[*] Checking memory...\n");
    delay();
    puts("[*] Analyzing processes...\n");
    delay();
    puts("[+] Booted normally.\n\n");
    delay();
}

void show_splash(void) {
    puts("       /\\        \n");
    puts("      /  \\       \n");
    puts("     / /\\ \\      \n");
    puts("    / ____ \\     \n");
    puts("   /_/    \\_\\    \n");
    puts("\n     Arc OS      \n");
    dbfs();
}

void kernel_main(void) {
    uart_init();  // initialize UART first

    show_splash();
    puts("\nWelcome to ArcOS!\n");
    puts("Log in as root user.\n");
    shell_main();

    while (1) {}
}

