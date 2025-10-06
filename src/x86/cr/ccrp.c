<<<<<<< HEAD
#include "input.h"
#include "print.h"

void println();

void main_cr() {
    println("ccrp> ");
    cinput();
    println("\n");
}
=======
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
>>>>>>> ac9702b722545892c836f5fb7eb49d29897b8750
