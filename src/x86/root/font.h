#ifndef FONT_H
#define FONT_H

#include <stdint.h>
#include <stddef.h>

// Load font from binary PSF (embedded via objcopy)
void init_font(void);

// Load font from C array
// font_data: pointer to font data (PSF format or raw glyphs)
// font_size: size of font data in bytes
// is_psf: if true, expects PSF header, if false, expects raw 16-byte glyphs
void init_font_from_array(const uint8_t* font_data, size_t font_size, int is_psf);

// Load font from raw glyph array (256 characters, 16 bytes each = 4096 bytes)
void init_font_from_glyphs(const uint8_t* glyphs);

#endif
