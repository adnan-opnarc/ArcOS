#include <stdint.h>
#include <stddef.h>
#include "font.h"

#include "../crp/io.h"
/* This matches the renamed symbol from the Makefile */
/* Declare as array to avoid array bounds warnings - actual size is determined at link time */
extern char _font_start[]; 

typedef struct {
    uint16_t magic;
    uint8_t  mode;
    uint8_t  char_size;
} psf1_header_t;

// Internal function to load glyphs to VGA
static void load_glyphs_to_vga(const unsigned char* glyphs) {

    // Unlock VGA font memory (plane 2) and map to 0xA0000
    // This sequence unlocks the font plane and allows writing to it
    
    // Sequence Control Register (0x3C4) - unlock font memory
    outb(0x3C4, 0x00); outb(0x3C5, 0x01);  // Reset register
    outb(0x3C4, 0x01); outb(0x3C5, 0x00);  // Synchronous reset off
    outb(0x3C4, 0x02); outb(0x3C5, 0x04);  // Map mask - enable plane 2 (font plane)
    outb(0x3C4, 0x04); outb(0x3C5, 0x07);  // Memory mode - disable chain 4, odd/even
    
    // Graphics Control Register (0x3CE) - configure for font access
    outb(0x3CE, 0x04); outb(0x3CF, 0x02);  // Read plane select - plane 2
    outb(0x3CE, 0x05); outb(0x3CF, 0x00);  // Graphics mode - normal
    outb(0x3CE, 0x06); outb(0x3CF, 0x04);  // Memory map - A0000-BFFFF

    // Copy font data to VGA font memory (0xA0000)
    // Each character is 16 bytes, stored at offset char_index * 32
    unsigned char* vga_mem = (unsigned char*)0xA0000;
    for (int i = 0; i < 256; i++) {
        for (int j = 0; j < 16; j++) {
            vga_mem[i * 32 + j] = glyphs[i * 16 + j];
        }
    }

    // Restore Normal Text Mode registers
    outb(0x3C4, 0x00); outb(0x3C5, 0x03);  // Reset register - normal
    outb(0x3C4, 0x01); outb(0x3C5, 0x00);  // Synchronous reset off
    outb(0x3C4, 0x02); outb(0x3C5, 0x03);  // Map mask - planes 0 and 1 (normal)
    outb(0x3C4, 0x04); outb(0x3C5, 0x03);  // Memory mode - normal
    outb(0x3CE, 0x04); outb(0x3CF, 0x00);  // Read plane select - plane 0
    outb(0x3CE, 0x05); outb(0x3CF, 0x10);  // Graphics mode - text mode
    outb(0x3CE, 0x06); outb(0x3CF, 0x0E);  // Memory map - B8000-BFFFF (text mode)
}

// Load font from embedded binary PSF
void init_font(void) {
    psf1_header_t* font = (psf1_header_t*)_font_start;
    
    // Safety check for PSF1 (0x0436 in little-endian, bytes are 0x36 0x04)
    if (font->magic != 0x0436) return;

    unsigned char* glyphs = (unsigned char*)font + sizeof(psf1_header_t);
    load_glyphs_to_vga(glyphs);
}

// Load font from C array (PSF format or raw glyphs)
void init_font_from_array(const uint8_t* font_data, size_t font_size, int is_psf) {
    if (!font_data || font_size < 4) return;
    
    const unsigned char* glyphs;
    
    if (is_psf) {
        // Check PSF1 magic
        psf1_header_t* font = (psf1_header_t*)font_data;
        if (font->magic != 0x0436) return;
        if (font_size < sizeof(psf1_header_t) + 256 * 16) return;
        glyphs = (unsigned char*)font + sizeof(psf1_header_t);
    } else {
        // Raw glyphs - expect exactly 256 * 16 = 4096 bytes
        if (font_size < 256 * 16) return;
        glyphs = font_data;
    }
    
    load_glyphs_to_vga(glyphs);
}

// Load font from raw glyph array (256 characters, 16 bytes each)
void init_font_from_glyphs(const uint8_t* glyphs) {
    if (!glyphs) return;
    load_glyphs_to_vga(glyphs);
}