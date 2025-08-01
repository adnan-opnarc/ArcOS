#include "keyboard.h"
#include "../../crp/arm/io.h"  // Your UART I/O functions here

extern int uart_getchar(void);  // Make sure uart_getchar() is implemented somewhere

void keyboard_init(void) {
    // Initialize UART if needed
    // uart_init();
}

char keyboard_getc(void) {
    int c = uart_getchar();  // Blocking read from UART
    return (char)c;
}

