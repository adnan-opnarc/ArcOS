/* include/uart.h */

#ifndef UART_H
#define UART_H

/**
 * @brief Initializes the UART (PL011) for serial communication on Raspberry Pi Zero 2 W.
 */
void uart_init(void);

/**
 * @brief Sends a single character over UART.
 * @param c The character to send.
 */
void uart_putc(unsigned char c);

/**
 * @brief Sends a null-terminated string over UART.
 * @param s The string to send.
 */
void uart_puts(const char* s);

#endif // UART_H
