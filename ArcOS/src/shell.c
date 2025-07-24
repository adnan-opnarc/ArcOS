#include "shell.h"
#include "keyboard.h"
#include "string.h"
#include "fat32.h"

#define MAX_COMMAND_LEN 100

// Forward declaration for your fake scan module
void msc_main(void);

// I/O port access for reboot/shutdown
static inline void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outw(unsigned short port, unsigned short val) {
    __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port));
}

// Shutdown via ACPI power off (QEMU, Bochs, VirtualBox compatible)
void arc_shutdown() {
    puts("System shutting down...\n");
    outw(0x604, 0x2000);  // QEMU/Bochs
    outw(0xB004, 0x2000); // VirtualBox
    while (1) { __asm__ volatile ("hlt"); }
}

// Reboot using keyboard controller
void arc_reboot() {
    puts("Rebooting system...\n");
    while ((inb(0x64) & 0x02) != 0); // wait until input buffer empty
    outb(0x64, 0xFE);                // send reset command
    while (1) { __asm__ volatile ("hlt"); }
}

void credit_main() {
    puts("Maintainer: Adnan\n");
    puts("Programmer: Adnan\n");
    puts("GNU GPL2 LICENSED\n");
}

void lfetch_main() {
    puts("       /\\        \n");
    puts("      /  \\       \n");
    puts("     / /\\ \\      \n");
    puts("    / ____ \\     \n");
    puts("   /_/    \\_\\    \n");
    puts("      ArcOS       \n");
    puts("=====================\n");
    puts("User: root\n");
    puts("Kernel: Artc v1.3\n");
    puts("CPU: x86 (32-bit)\n");
    puts("Shell: ArcShell\n");
    puts("License: GPLv2\n");
}
void whmi_main() {
    puts("root\n");
}

void shv_v() {
    puts("1.2kfv\n");
}

void shell_main(void) {
    char command[MAX_COMMAND_LEN];
    int i = 0;

    puts("root@adm# ");

    while (1) {
        char c = keyboard_getc();
        if (c) {
            if (c == '\n') {
                command[i] = '\0';
                putchar('\n');

                if (i == 0) {
                    // empty command, just prompt again
                } else if (strcmp(command, "help") == 0) {
                    puts("Available commands: credit, help, clear, ls, msc, reboot, shutdown, lfetch, whoami, shell --v\n");
                } else if (strcmp(command, "credit") == 0) {
                    credit_main();
                } else if (strcmp(command, "clear") == 0) {
                    clear_screen();
                } else if (strcmp(command, "ls") == 0) {
                    struct fat32_file files[16];
                    int count = fat32_list_dir(files, 16);
                    for (int j = 0; j < count; j++) {
                        puts(files[j].name);
                        putchar('\n');
                    }
                } else if (strcmp(command, "msc") == 0) {
                    msc_main();
                } else if (strcmp(command, "shutdown") == 0) {
                    arc_shutdown();
                } else if (strcmp(command, "reboot") == 0) {
                    arc_reboot();
                } else if (strcmp(command, "lfetch") == 0) {
                    lfetch_main();
                } else if (strcmp(command,  "whoami") ==0) {
                    whmi_main();                
                } else if (strcmp(command,  "shell --v") == 0) {
                    shv_v();
                } else {
                    puts("Unknown command: ");
                    puts(command);
                    putchar('\n');
                }

                i = 0;
                puts("root@adm# ");
            } else if (c == '\b') {
                if (i > 0) {
                    i--;
                    puts("\b \b");  // properly erase char on terminal
                }
            } else {
                if (i < MAX_COMMAND_LEN - 1) {
                    command[i++] = c;
                    putchar(c);
                }
            }
        }
    }
}

