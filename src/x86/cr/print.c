#include <stdint.h>
#include "../root/kernel.h"   // for putchar()

void putchar(char c);

void println(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}
