#ifndef SHELL_H
#define SHELL_H
#include <stdbool.h>

void printk(const char* str);
void putchar(char c);
void clear_screen(void);
void shell_main(void);
void arc_shutdown(void);
<<<<<<< HEAD
void input_sh(char* buffer, int max_len, bool hide_input);
=======
void input(char* buffer, int max_len, bool hide_input);
>>>>>>> ac9702b722545892c836f5fb7eb49d29897b8750

#endif 