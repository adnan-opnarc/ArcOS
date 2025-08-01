/* src/drivers/uart.c */

#include "uart.h" // Include our UART header

// Peripheral base address for the PL011 UART on Raspberry Pi (BCM2710A1)
// The actual base is 0x7E201000 in the ARM physical address space,
// but for RPi models (like RPi3/Zero 2 W) the memory is remapped
// to be accessible starting at 0x3F000000 in the low peripheral area.
// So, the UART base becomes 0x3F201000.
#define UART_BASE 0x3F201000

// PL011 UART registers (offsets from UART_BASE)
#define UART_DR     ((volatile unsigned int*)(UART_BASE + 0x00)) // Data Register
#define UART_FR     ((volatile unsigned int*)(UART_BASE + 0x18)) // Flag Register
#define UART_IBRD   ((volatile unsigned int*)(UART_BASE + 0x24)) // Integer Baud Rate Divisor
#define UART_FBRD   ((volatile unsigned int*)(UART_BASE + 0x28)) // Fractional Baud Rate Divisor
#define UART_LCRH   ((volatile unsigned int*)(UART_BASE + 0x2C)) // Line Control Register
#define UART_CR     ((volatile unsigned int*)(UART_BASE + 0x30)) // Control Register
#define UART_IMSC   ((volatile unsigned int*)(UART_BASE + 0x38)) // Interrupt Mask Set/Clear Register
#define UART_ICR    ((volatile unsigned int*)(UART_BASE + 0x44)) // Interrupt Clear Register

// Flag Register bits
#define UART_FR_TXFF    (1 << 5) // Transmit FIFO Full
#define UART_FR_RXFE    (1 << 4) // Receive FIFO Empty
#define UART_FR_BUSY    (1 << 3) // UART Busy

// GPIO base address for configuring UART pins
// Same remapping principle applies. GPIO base is 0x3F200000.
#define GPIO_BASE 0x3F200000

// GPIO Function Select registers
#define GPFSEL1 ((volatile unsigned int*)(GPIO_BASE + 0x04)) // Controls GPIO pins 10-19

// Baud rate calculation for 3MHz clock (common on RPi for PL011, check docs!)
// Baudrate divisor = UART_CLOCK / (16 * Baudrate)
// For 115200 baud and 3MHz clock: 3000000 / (16 * 115200) = 1.6276
// IBRD = floor(1.6276) = 1
// FBRD = round(0.6276 * 64) = 40.1664 ~= 40
#define UART_CLOCK      3000000 // Placeholder, verify with RPi documentation!
#define BAUD_RATE       115200

/**
 * @brief Initializes the UART (PL011) for serial communication.
 */
void uart_init(void) {
    register unsigned int r;

    // Disable UART, clear interrupts, disable pull-ups on GPIO
    *UART_CR = 0; // Disable UART for configuration
    *UART_IMSC = 0; // Disable all interrupts
    *UART_ICR = 0x7FF; // Clear all pending interrupts

    // Configure GPIO pins 14 (TX) and 15 (RX) for UART alternate function 0 (ALT0)
    // GPFSEL1 controls pins 10-19.
    // Pin 14 is bits 12-14. Pin 15 is bits 15-17.
    // ALT0 is function code 4 (binary 100).
    // Clear existing settings for GPIO14 & GPIO15
    r = *GPFSEL1;
    r &= ~((7 << 12) | (7 << 15)); // Clear bits for GPIO14 and GPIO15
    r |= ((4 << 12) | (4 << 15));  // Set ALT0 (0b100)
    *GPFSEL1 = r;

    // (Optional but good practice) Disable pull-up/down for GPIO14/15
    // For BCM2710A1 (RPi3/Zero 2W), this uses a different mechanism than BCM2835.
    // Modern Pi's often handle this via the firmware, but for bare-metal:
    // This is more complex on AArch64 and involves GPPUD / GPPUDCLK registers for older Pis.
    // For BCM2710A1, the recommended way is usually to let the firmware handle it,
    // or use GPIO PULL (GPPUPDN) registers, which are different from BCM2835.
    // For this minimal example, we'll omit explicit pull-up/down for now as bootloader usually sets it.

    // Set baud rate (integer and fractional parts)
    // IBRD = UART_CLOCK / (16 * BAUD_RATE)
    // FBRD = round((fractional part) * 64)
    *UART_IBRD = 1; // From calculation: floor(1.6276) = 1
    *UART_FBRD = 40; // From calculation: round(0.6276 * 64) = 40

    // Set line control: 8 data bits, no parity, 1 stop bit, enable FIFOs
    // (8 bits, Enable FIFOs: 0b11 << 5 | 0b10000)
    *UART_LCRH = (1 << 4) | (1 << 5) | (1 << 6); // Word length 8 bits, Enable FIFOs

    // Enable UART, TX, and RX
    *UART_CR = (1 << 0) | (1 << 8) | (1 << 9); // UARTEN | TXE | RXE
}

/**
 * @brief Sends a single character over UART.
 * @param c The character to send.
 */
void uart_putc(unsigned char c) {
    // Wait until TX FIFO is not full
    while (*UART_FR & UART_FR_TXFF) {
        // Spin
    }
    *UART_DR = c; // Write character to data register
}

/**
 * @brief Sends a null-terminated string over UART.
 * @param s The string to send.
 */
void uart_puts(const char* s) {
    while (*s) {
        if (*s == '\n') {
            uart_putc('\r'); // Add carriage return for newlines for typical terminals
        }
        uart_putc(*s++);
    }
}
