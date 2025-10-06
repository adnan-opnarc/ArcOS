#include "../root/kernel.h"
#include "../sh/shell.h"
#include "print.h"
#include <stdbool.h>


void cinput(void) {
    char buffer[64]; 
    input_sh(buffer, sizeof(buffer), false);  // call the shell's input function
    println(buffer);
}
