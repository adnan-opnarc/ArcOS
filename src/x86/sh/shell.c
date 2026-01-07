#include "cd.h"
#include "echo.h"
#include "exit.h"
#include "lfetch.h"
#include "man.h" //manual entry for kernel info and dev opt
#include "ls.h"
#include "ltime.h"
#include "mkdir.h"
#include "rmdir.h"
#include "bitmap.h"
#include "cat.h"
#include "touch.h"
#include "script.h"
#include "shell.h"
#include "../driver/keyboard.h"
#include "../crp/string.h"
#include "../crp/io.h"
#include "../cr/ccrp.h"
#include "../fs/ramfs.h"
#include "../root/tui.h"
#include "pid.h"
#include <stdbool.h>
#include <stdint.h>
#define MAX_COMMAND_LEN 256



// Sets the shape of the cursor
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

// Moves the hardware cursor to a specific X, Y
void move_cursor(int x, int y) {
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// Reads the current hardware cursor position and "refreshes" it
// This prevents the undefined reference to terminal_row/column
void sync_cursor_hardware() {
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;

    // Write it back to ensure it's visible/blinking
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// --- 3. SHELL UTILITIES ---

void credit_main(void) {
    printk("Maintainer:Adnan\nProgrammer:Adnan\n");
}

void whmi_main(void) {
    printk("root\n");
}

void shv_v(void) {
    printk("0kvs\n");
}

void arc_shutdown() {
    printk("system closing\n");
    outw(0x604, 0x2000);  // QEMU/Bochs
    outw(0xB004, 0x2000); // VirtualBox
    while (1) { __asm__ volatile ("hlt"); }
}

void arc_reboot() {
    printk("Rebooting system...\n");
    while ((inb(0x64) & 0x02) != 0); 
    outb(0x64, 0xFE);                
    while (1) { __asm__ volatile ("hlt"); }
}

// --- 4. INPUT LOGIC ---

void input(char* buffer, int max_len, bool hide_input) {
    int i = 0;
    while (1) {
        sync_cursor_hardware(); 
        char c = keyboard_getc();
        if (!c) continue;

        if (c == '\n' || c == '\r') {
            putchar('\n');
            buffer[i] = '\0';
            break;
        } else if (c == '\b') {
            if (i > 0) {
                i--;
                printk("\b \b"); 
                sync_cursor_hardware();
            }
        } else if (i < max_len - 1) {
            buffer[i++] = c;
            if (hide_input) {
                putchar('*');
            } else {
                putchar(c);
            }
            sync_cursor_hardware();
        }
    }
}

void input_sh(char* buffer, int max_len, bool hide_input) {
    input(buffer, max_len, hide_input);
}

// --- 5. AUTHENTICATION ---

void main_cr(void);

static bool check_credentials(const char* user, const char* pass) {
    return strcmp(user, "root") == 0 && strcmp(pass, "arcroot") == 0;
}

static void login_prompt(void) {
    char username[32];
    char password[32];

    while (1) {
        printk("Username: ");
        input_sh(username, sizeof(username), false);

        printk("Password: ");
        input_sh(password, sizeof(password), true);

        if (check_credentials(username, password)) {
            clear_screen();
            printk("\nKernel version 1.5 Archaeopatryx-15/8/2025");
            printk("\nLogin successful. Type 'help' command to get all the commands listed\n");
            break;
        } else {
            printk("\nLogin failed. Try again.\n");
        }
    }
}

// --- 6. MAIN SHELL LOOP ---

void shell_main(void) {
    // Enable blinking underline cursor (scanlines 13-14)
    enable_cursor(13, 14); 
    
    login_prompt();

    char command[MAX_COMMAND_LEN];
    int i = 0;

    printk("/home-$ ");

    while (1) {
        sync_cursor_hardware();
        char c = keyboard_getc();
        if (c) {
            if (c == '\n' || c == '\r') {
                command[i] = '\0';
                putchar('\n');

                if (i == 0) {
                    // Just print prompt again
                } else if (strcmp(command, "pid") == 0){
                    pid_main();
                } else if (strcmp(command, "help") == 0) {
                    printk("Available commands:\n");
                    printk("  ls - list files\n");
                    printk("  mkdir - create directory\n");
                    printk("  rmdir - remove directory\n");
                    printk("  cat <file> - read file\n");
                    printk("  touch <file> - create file\n");
                    printk("  echo <text> - print text\n");
                    printk("  echo > <file> <text> - write to file\n");
                    printk("  sh <script> - run script\n");
                    printk("  clear, help, reboot, shutdown, bitmap\n");
                } else if (strcmp(command, "credit") == 0) {
                    credit_main();
                } else if (strcmp(command, "exit") == 0){
                    ui_start();
                } else if (strcmp(command, "clear") == 0) {
                    clear_screen();
                } else if (strcmp(command, "ls") == 0) {
                    ls_main();
                } else if (strcmp(command, "shutdown") == 0) {
                    arc_shutdown();
                } else if (strcmp(command, "reboot") == 0) {
                    arc_reboot();
                } else if (strcmp(command, "lfetch") == 0) {
                    lfetch_main();
                } else if (strcmp(command, "whoami") == 0) {
                    whmi_main();                
                } else if (strcmp(command, "shell --v") == 0) {
                    shv_v();
                } else if (strcmp(command, "mkdir") == 0) {
                    mkdir_main();
                } else if (strcmp(command, "rmdir") == 0) {
                    rmdir_main();
                } else if (strcmp(command, "ccrp") == 0) {
                    main_cr();
                } else if (strcmp(command, "man") == 0) {
                    main_man();
                } else if (strncmp(command, "bitmap", 6) == 0) {
                    // Pass arguments after "bitmap "
                    const char* args = command + 6;
                    while (*args == ' ') args++;  // Skip spaces
                    bitmap_main(args);
                } else if (strncmp(command, "cat ", 4) == 0) {
                    const char* filename = command + 4;
                    while (*filename == ' ') filename++;  // Skip spaces
                    cat_main(filename);
                } else if (strncmp(command, "touch ", 6) == 0) {
                    const char* filename = command + 6;
                    while (*filename == ' ') filename++;  // Skip spaces
                    touch_main(filename);
                } else if (strncmp(command, "sh ", 3) == 0 || strncmp(command, "./", 2) == 0) {
                    // Execute script: sh <script> or ./<script>
                    const char* script_path = command;
                    if (strncmp(command, "sh ", 3) == 0) {
                        script_path = command + 3;
                    } else {
                        script_path = command + 2;
                    }
                    while (*script_path == ' ') script_path++;  // Skip spaces
                    script_execute(script_path);
                } else if (strncmp(command, "echo ", 5) == 0) {
                    const char* text = command + 5;
                    while (*text == ' ') text++;  // Skip spaces
                    if (strncmp(text, "> ", 2) == 0) {
                        // File redirection: echo > file.txt "text"
                        const char* filename = text + 2;
                        while (*filename == ' ') filename++;
                        // Find the text after filename
                        const char* file_text = filename;
                        // Skip filename to find text
                        while (*file_text != ' ' && *file_text != '\0') file_text++;
                        if (*file_text == ' ') {
                            file_text++;  // Skip space
                            char fname[64];
                            int i = 0;
                            const char* fname_start = filename;
                            while (*fname_start != ' ' && *fname_start != '\0' && i < 63) {
                                fname[i++] = *fname_start++;
                            }
                            fname[i] = '\0';
                            // Write to file
                            ramfs_write(fname, (const uint8_t*)file_text, strlen(file_text));
                            printk("Written to ");
                            printk(fname);
                            putchar('\n');
                        }
                    } else {
                        // Normal echo
                        printk(text);
                        putchar('\n');
                    }
                }
                else {
                    printk("Unknown command: ");
                    printk(command);
                    putchar('\n');
                }

                i = 0;
                printk("/home-$ ");
                sync_cursor_hardware();
            } else if (c == '\b') {
                if (i > 0) {
                    i--;
                    printk("\b \b"); 
                    sync_cursor_hardware();
                }
            } else {
                if (i < MAX_COMMAND_LEN - 1) {
                    command[i++] = c;
                    putchar(c);
                    sync_cursor_hardware();
                }
            }
        }
    }
}