#include "rmdir.h"
#include "../fs/ramfs.h"
#include "shell.h"

void rmdir_main(void) {
    // For now, remove /home directory as example
    // In a full implementation, this would parse arguments
    if (ramfs_delete("/home") == 0) {
        printk("Directory removed: /home\n");
    } else {
        printk("Directory not found or cannot be removed\n");
    }
}

