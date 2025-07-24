#ifndef TIMER_H
#define TIMER_H

// Global tick counter (updated by PIT)
extern unsigned int tick_count;

// Init function to start PIT
void init_timer(unsigned int frequency);

// Optional: timer interrupt handler
void timer_callback(void);

#endif

