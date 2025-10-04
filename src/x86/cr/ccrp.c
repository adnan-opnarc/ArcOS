#include "../sh/shell.h"
#include "../root/kernel.h"

// CCRP interpreter entry point
void main_cr(void) {
    char buffer[64];

    printk("ccrp> ");  // prompt

    // use the same input() from shell
    input(buffer, sizeof(buffer), false);

    printk("You typed: ");
    printk(buffer);
    printk("\n");
}
