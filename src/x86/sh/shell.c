#include "cd.h"
#include "echo.h"
#include "exit.h"
#include "lfetch.h"
#include "ls.h"
#include "ltime.h"
#include "mkdir.h"
#include "rmdir.h"
#include "shell.h"
#include "../driver/keyboard.h"
#include "../crp/string.h"
#include "../fs/fat32.h"
#include "../cr/ccrp.h"
#include <stdbool.h>  // added for bool

#define MAX_COMMAND_LEN 256


// Declare external functions implemented elsewhere
void credit_main(void) {
    printk("Maintainer:Adnan\nProgrammer:Adnan\n");
};

void whmi_main(void) {
    printk("root\n");
};
void shv_v(void) {
    printk("0kvs\n");
};

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
    printk("system closing\n");
    outw(0x604, 0x2000);  // QEMU/Bochs
    outw(0xB004, 0x2000); // VirtualBox
    while (1) { __asm__ volatile ("hlt"); }
}

// Reboot using keyboard controller
void arc_reboot() {
    printk("Rebooting system...\n");
    while ((inb(0x64) & 0x02) != 0); // wait until input buffer empty
    outb(0x64, 0xFE);                // send reset command
    while (1) { __asm__ volatile ("hlt"); }
}

// Helper: get input from keyboard, optionally hiding input (for password)
<<<<<<< HEAD
void input_sh(char* buffer, int max_len, bool hide_input) {
=======
void input(char* buffer, int max_len, bool hide_input) {
>>>>>>> ac9702b722545892c836f5fb7eb49d29897b8750
    int i = 0;
    while (1) {
        char c = keyboard_getc();
        if (!c) continue;

        if (c == '\n' || c == '\r') {
            putchar('\n');
            buffer[i] = '\0';
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                printk("\b \b"); // erase character on screen
            }
        } else if (i < max_len - 1) {
            buffer[i++] = c;
            if (hide_input) {
                putchar('*');
            } else {
                putchar(c);
            }
        }
    }
}
//cryptic interpreter for the language changes
void main_cr(void);
// Check if entered username and password match expected values
static bool check_credentials(const char* user, const char* pass) {
    return strcmp(user, "root") == 0 && strcmp(pass, "arcroot") == 0;
}

// Login prompt: loops until correct credentials entered
static void login_prompt(void) {
    char username[32];
    char password[32];

    while (1) {
        printk("Username: ");
<<<<<<< HEAD
        input_sh(username, sizeof(username), false);

        printk("Password: ");
        input_sh(password, sizeof(password), true);
=======
        input(username, sizeof(username), false);

        printk("Password: ");
        input(password, sizeof(password), true);
>>>>>>> ac9702b722545892c836f5fb7eb49d29897b8750

        if (check_credentials(username, password)) {
            clear_screen();
            printk("\nKernel version 1.5 Archaeopatryx-15/8/2025");
            printk("\nLogin successful.Type 'help' command to get all the commands listed\n");
            break;
        } else {
            printk("\nLogin failed. Try again.\n");
        }
    }
}

void shell_main(void) {
    login_prompt();

    char command[MAX_COMMAND_LEN];
    int i = 0;

    printk("root@adm# ");

    while (1) {
        char c = keyboard_getc();
        if (c) {
            if (c == '\n' || c == '\r') {
                command[i] = '\0';
                putchar('\n');

                if (i == 0) {
                } else if (strcmp(command, "help") == 0) {
                    printk("Available commands: ccrp, mkdir, rmdir, credit, help, clear, ls, reboot, shutdown, lfetch, whoami, shell --v\n");
                } else if (strcmp(command, "credit") == 0) {
                    credit_main();
                } else if (strcmp(command, "clear") == 0) {
                    clear_screen();
                } else if (strcmp(command, "ls") == 0) {
                    struct fat32_file files[16];
                    int count = fat32_list_dir(files, 16);
                    for (int j = 0; j < count; j++) {
                        printk(files[j].name);
                        putchar('\n');
                    }
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
                } else if (strcmp(command,  "mkdir") == 0) {
                    mkdir_main();
                } else if (strcmp(command,  "rmdir") == 0) {
                    rmdir_main();
                } else if (strcmp(command, "ccrp") == 0) {
                    main_cr();
                } else {
                    printk("Unknown command: ");
                    printk(command);
                    putchar('\n');
                }

                i = 0;
                printk("root@adm# ");
            } else if (c == '\b') {
                if (i > 0) {
                    i--;
                    printk("\b \b"); 
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

