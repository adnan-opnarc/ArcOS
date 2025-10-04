#include "../sh/shell.h"
#include "kernel.h"

void delay(void);
void stdelay(void);
void arc_shutdown(void);

void kernel_panic() {
    clear_screen();
    printk("[0.12] boot completed\n");
    delay();
    printk("[0.999921] kernel entry - 0x1000000\n");
    delay();
    printk("[0.0]kernel-no shell\n");
    delay();
    printk("[0.0001] kernel panic - not syncing: Attempted to kill init!\n");
    delay();
    printk("[0.0002] CPU: 0 PID: 1 Comm: swapper Not tainted 1.4k-generic #46\n");
    delay();
    printk("[0.0003] Hardware name: Unknown\n");
    delay();
    printk("0x102090    0x18    0x7280  0x8399999\n");
    printk("halting the session\n");
    delay();
    printk("in 3");
    delay();
    printk(" 2");
    delay();
    printk(" 1\n");
    delay();
    arc_shutdown();
}
