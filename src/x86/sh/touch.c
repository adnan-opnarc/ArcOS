#include "touch.h"
#include "../fs/ramfs.h"
#include "shell.h"
#include "../crp/string.h"

void touch_main(const char* filename) {
    if (!filename || filename[0] == '\0') {
        printk("Usage: touch <filename>\n");
        return;
    }
    
    // Create empty file or update if exists
    if (ramfs_create_file(filename) == 0) {
        printk("File created: ");
        printk(filename);
        putchar('\n');
    } else {
        // File might already exist, try to write empty data
        uint8_t empty = 0;
        if (ramfs_write(filename, &empty, 0) == 0) {
            printk("File updated: ");
            printk(filename);
            putchar('\n');
        } else {
            printk("Failed to create file\n");
        }
    }
}

