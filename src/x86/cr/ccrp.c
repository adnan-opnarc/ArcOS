#include "../sh/shell.h"
#include "../root/kernel.h"
#include "print.h"

void main_cr(void) {
    char buffer[64];
    println("ccrp> ");  // prompt
    input(buffer, sizeof(buffer), false);
    println("You typed: ");
    println(buffer);
    println("\n");
}
