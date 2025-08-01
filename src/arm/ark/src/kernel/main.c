/* src/kernel/main.c */

#include "uart.h"

// Define the main function that the assembly code will call
void kernel_main(void) {
    uart_init(); // Initialize the UART
    uart_puts("Hello from ARK on Raspberry Pi Zero 2 W!\n"); // Print a message
    uart_puts("AArch64 Bare Metal Kernel is running...\n");

    // Infinite loop to prevent the kernel from exiting (which would halt the CPU)
    while (1) {
        // You can add more kernel logic here later
    }
}
