#ifndef UART_H
#define UART_H

void uart_init(void);
void uart_send(char c);
char uart_recv(void);
int uart_ready(void);

#endif

