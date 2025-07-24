#include "timer.h"
#include "isr.h"
#include "ports.h"

unsigned int tick_count = 0;

void timer_callback() {
    tick_count++;
    // Optionally: do something every second
}

void init_timer(unsigned int frequency) {
    // Register timer_callback as IRQ0
    register_interrupt_handler(32, timer_callback); // assuming IRQ0 mapped to int 32

    // Setup PIT
    unsigned int divisor = 1193180 / frequency;
    outb(0x43, 0x36);
    outb(0x40, divisor & 0xFF);
    outb(0x40, (divisor >> 8) & 0xFF);
}

