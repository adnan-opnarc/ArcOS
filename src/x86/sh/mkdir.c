#include "mkdir.h"
#include "../fs/ramfs.h"
#include "shell.h"
#include "../crp/string.h"

void mkdir_main(void) {
    // For now, create a default directory
    // In a full implementation, this would parse arguments
    if (ramfs_mkdir("/home") == 0) {
        printk("Directory created: /home\n");
    } else {
        printk("Failed to create directory or already exists\n");
    }
}

