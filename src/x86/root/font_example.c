/*
 * Example: Using C array fonts
 * 
 * This file demonstrates how to use fonts from C arrays instead of binary PSF files.
 * You can uncomment the example code and include it in your kernel.
 */

#include "font.h"
#include "font_array.h"  // Generated from psf_to_carray.py

// Example 1: Load font from PSF C array
void example_load_psf_array(void) {
    // Load from PSF format C array (includes header)
    init_font_from_array(font_array, FONT_ARRAY_SIZE, 1);  // 1 = is_psf
}

// Example 2: Load font from raw glyphs C array
void example_load_raw_glyphs(void) {
    // Load from raw glyphs (skip PSF header)
    init_font_from_glyphs(FONT_ARRAY_GLYPHS);
}

// Example 3: Load font from custom C array
void example_load_custom_array(void) {
    // You can define your own font array:
    static const uint8_t my_custom_font[] = {
        // PSF header (4 bytes)
        0x36, 0x04, 0x00, 0x10,
        // Glyph data (256 * 16 = 4096 bytes)
        // ... your glyph data here ...
    };
    
    init_font_from_array(my_custom_font, sizeof(my_custom_font), 1);
}

// Example 4: Load font from raw glyphs only
void example_load_raw_only(void) {
    // Raw glyphs without PSF header (4096 bytes)
    static const uint8_t raw_glyphs[256 * 16] = {
        // ... 4096 bytes of glyph data ...
    };
    
    init_font_from_glyphs(raw_glyphs);
}

