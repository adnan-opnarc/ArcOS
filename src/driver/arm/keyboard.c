#include <stdint.h>
#include <stdbool.h>
#include "keyboard.h"
#include "../../crp/arm/io.h"  // Replace with your UART I/O header

extern int uart_getchar(void); // You need to implement this for your UART

static bool shift = false;
static bool caps = false;

void keyboard_init(void) {
    shift = false;
    caps = false;
    // uart_init(); // Init your UART if needed
}

char keyboard_getc(void) {
    int c = uart_getchar(); // Blocking read from UART

    // No shift or caps handling for now (optional: implement terminal escape sequences)
    return (char)c;
}

