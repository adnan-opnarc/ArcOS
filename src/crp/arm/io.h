#ifndef IO_H
#define IO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    (void)port;
    (void)val;
}

static inline uint8_t inb(uint16_t port) {
    (void)port;
    return 0;
}

static inline void insw(uint16_t port, void* addr, int count) {
    (void)port;
    (void)addr;
    (void)count;
}

static inline void outsw(uint16_t port, const void* addr, int count) {
    (void)port;
    (void)addr;
    (void)count;
}

#endif

