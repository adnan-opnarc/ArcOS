#include <stdint.h>
#include "ata.h"

void ata_init(void) {
    // no ATA on ARM yet
}

int ata_read_sector(uint32_t lba, void* buf) {
    (void)lba;
    (void)buf;
    return -1;  // not supported
}

int ata_write_sector(uint32_t lba, const void* buf) {
    (void)lba;
    (void)buf;
    return -1;  // not supported
}

