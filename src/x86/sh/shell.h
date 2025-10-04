#ifndef SHELL_H
#define SHELL_H
#include <stdbool.h>

void printk(const char* str);
void putchar(char c);
void clear_screen(void);
void shell_main(void);
void arc_shutdown(void);
void input(char* buffer, int max_len, bool hide_input);

#endif 