#include "cat.h"
#include "../fs/ramfs.h"
#include "shell.h"
#include "../crp/string.h"

void cat_main(const char* filename) {
    if (!filename || filename[0] == '\0') {
        printk("Usage: cat <filename>\n");
        return;
    }
    
    uint8_t buffer[1024];
    uint32_t bytes_read = 0;
    
    if (ramfs_read(filename, buffer, sizeof(buffer) - 1, &bytes_read) == 0) {
        buffer[bytes_read] = '\0'; // Null terminate
        printk((const char*)buffer);
        putchar('\n');
    } else {
        printk("File not found: ");
        printk(filename);
        putchar('\n');
    }
}

