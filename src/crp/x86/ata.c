#include <stdint.h>
#include "ata.h"
#include "io.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_PRIMARY_CTRL 0x3F6

static inline void io_wait(void) {
    for (volatile int i = 0; i < 1000; i++);
}

void ata_init(void) {
    // No-op for PIO
}

int ata_read_sector(uint32_t lba, void* buf) {
    outb(ATA_PRIMARY_CTRL, 0); // nIEN = 0
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F)); // drive/head
    outb(ATA_PRIMARY_IO + 2, 1); // sector count
    outb(ATA_PRIMARY_IO + 3, lba & 0xFF);
    outb(ATA_PRIMARY_IO + 4, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + 5, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_IO + 7, 0x20); // READ SECTORS
    // Wait for BSY=0, DRQ=1
    uint8_t status;
    do { status = inb(ATA_PRIMARY_IO + 7); } while (status & 0x80);
    if (!(status & 0x08)) return -1;
    insw(ATA_PRIMARY_IO, buf, 256);
    io_wait();
    return 0;
}

int ata_write_sector(uint32_t lba, const void* buf) {
    outb(ATA_PRIMARY_CTRL, 0); // nIEN = 0
    outb(ATA_PRIMARY_IO + 6, 0xE0 | ((lba >> 24) & 0x0F)); // drive/head
    outb(ATA_PRIMARY_IO + 2, 1); // sector count
    outb(ATA_PRIMARY_IO + 3, lba & 0xFF);
    outb(ATA_PRIMARY_IO + 4, (lba >> 8) & 0xFF);
    outb(ATA_PRIMARY_IO + 5, (lba >> 16) & 0xFF);
    outb(ATA_PRIMARY_IO + 7, 0x30); // WRITE SECTORS
    // Wait for BSY=0, DRQ=1
    uint8_t status;
    do { status = inb(ATA_PRIMARY_IO + 7); } while (status & 0x80);
    if (!(status & 0x08)) return -1;
    outsw(ATA_PRIMARY_IO, buf, 256);
    io_wait();
    return 0;
} 