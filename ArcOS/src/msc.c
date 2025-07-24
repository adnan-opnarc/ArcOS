#include <stdio.h>
#include <unistd.h>  // for sleep()
#include "msc.h"

void msc_main() {
    printf("[OK] boot parameter\n");
    sleep(1);
    printf("[ok]fat32 init\n");
    sleep(1);
    printf("[--]network driver failed to init\n");
    sleep(1);
    printf("[SYSC COMPLETED 1 ERROR FOUND]\n");
}

