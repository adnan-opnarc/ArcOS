#include "uart.h"

#define UART0_BASE 0x09000000

#define UART_DR     (*(volatile unsigned int*)(UART0_BASE + 0x00))
#define UART_FR     (*(volatile unsigned int*)(UART0_BASE + 0x18))
#define UART_IBRD   (*(volatile unsigned int*)(UART0_BASE + 0x24))
#define UART_FBRD   (*(volatile unsigned int*)(UART0_BASE + 0x28))
#define UART_LCRH   (*(volatile unsigned int*)(UART0_BASE + 0x2C))
#define UART_CR     (*(volatile unsigned int*)(UART0_BASE + 0x30))
#define UART_IMSC   (*(volatile unsigned int*)(UART0_BASE + 0x38))
#define UART_ICR    (*(volatile unsigned int*)(UART0_BASE + 0x44))

void uart_init(void) {
    // Disable UART0
    UART_CR = 0x00000000;

    // Clear all interrupts
    UART_ICR = 0x7FF;

    // Set integer & fractional part of baud rate
    UART_IBRD = 1;    // for 115200 baud if UART clock = 3MHz
    UART_FBRD = 40;

    // Set to 8 bits, no parity, one stop bit, FIFOs enabled
    UART_LCRH = (1 << 4) | (3 << 5);

    // Enable UART0, TX, RX
    UART_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_send(char c) {
    // Wait until TX is ready
    while (UART_FR & (1 << 5)) {}  // TXFF
    UART_DR = c;
}

char uart_recv(void) {
    // Wait until RX is ready
    while (UART_FR & (1 << 4)) {}  // RXFE
    return UART_DR;
}

int uart_ready(void) {
    return !(UART_FR & (1 << 4));  // 1 if data is available
}

