#include "ls.h"
#include "../fs/ramfs.h"
#include "shell.h"
#include "../crp/string.h"

void ls_main(void) {
    char output[512];
    int count = ramfs_list("/", output, sizeof(output));
    
    if (count > 0) {
        printk(output);
        putchar('\n');
    } else {
        printk("(empty)\n");
    }
}

