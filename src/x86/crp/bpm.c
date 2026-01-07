/*
 * Example: Using custom bitmaps from C arrays
 * 
 * This demonstrates how to create and use bitmaps from C arrays
 * in any system function.
 */

#include "bitmap.h"

// Example 1: Simple monochrome bitmap (8x8 pixels)
// Each byte represents 8 pixels (1 bit per pixel)
static const uint8_t icon_bitmap_data[] = {
    0b00011000,  // Row 0: ...##...
    0b00111100,  // Row 1: ..####..
    0b01111110,  // Row 2: .######.
    0b11111111,  // Row 3: ########
    0b11111111,  // Row 4: ########
    0b01111110,  // Row 5: .######.
    0b00111100,  // Row 6: ..####..
    0b00011000   // Row 7: ...##...
};

// Example 2: Indexed color bitmap (16x16 pixels, 1 byte per pixel)
static const uint8_t sprite_data[] = {
    // 16x16 = 256 bytes of indexed pixel data
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
    0, 0, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0,
    0, 1, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 1, 0,
    // ... more rows ...
};

// Example 3: Palette for indexed bitmap (4 colors, RGB format)
static const uint8_t sprite_palette[] = {
    0x00, 0x00, 0x00,  // Color 0: Black
    0xFF, 0x00, 0x00,  // Color 1: Red
    0x00, 0xFF, 0x00,  // Color 2: Green
    0x00, 0x00, 0xFF   // Color 3: Blue
};

// Example 4: RGB bitmap (4x4 pixels, 3 bytes per pixel)
static const uint8_t rgb_bitmap_data[] = {
    // Pixel (0,0): Red
    0xFF, 0x00, 0x00,
    // Pixel (1,0): Green
    0x00, 0xFF, 0x00,
    // Pixel (2,0): Blue
    0x00, 0x00, 0xFF,
    // Pixel (3,0): White
    0xFF, 0xFF, 0xFF,
    // ... more pixels ...
};

// Example usage functions
void example_monochrome_bitmap(void) {
    // Create monochrome bitmap from C array
    bitmap_t icon = bitmap_create(
        icon_bitmap_data,
        sizeof(icon_bitmap_data),
        8, 8,
        BITMAP_FORMAT_MONO
    );
    
    if (bitmap_is_valid(&icon)) {
        // Access pixels
        uint8_t pixel = bitmap_get_pixel_raw(&icon, 3, 3);  // Get pixel at (3,3)
        // pixel will be 1 if set, 0 if not
    }
}

void example_indexed_bitmap(void) {
    // Create indexed bitmap with palette
    bitmap_t sprite = bitmap_create_indexed(
        sprite_data,
        sizeof(sprite_data),
        16, 16,
        sprite_palette,
        4  // 4 colors in palette
    );
    
    if (bitmap_is_valid(&sprite)) {
        // Get pixel as RGB
        uint8_t r, g, b;
        bitmap_get_pixel_rgb(&sprite, 5, 5, &r, &g, &b);
        // r, g, b now contain the RGB values from the palette
    }
}

void example_rgb_bitmap(void) {
    // Create RGB bitmap
    bitmap_t rgb_bmp = bitmap_create(
        rgb_bitmap_data,
        sizeof(rgb_bitmap_data),
        4, 4,
        BITMAP_FORMAT_RGB
    );
    
    if (bitmap_is_valid(&rgb_bmp)) {
        uint8_t r, g, b;
        bitmap_get_pixel_rgb(&rgb_bmp, 0, 0, &r, &g, &b);
        // r=255, g=0, b=0 (red)
    }
}

// Example: Custom bitmap for UI element
static const uint8_t button_bitmap[] = {
    // 32x16 monochrome button
    0xFF, 0xFF, 0xFF, 0xFF,  // Row 0: all pixels on
    0x81, 0x00, 0x00, 0x81,  // Row 1: edges only
    0x81, 0x00, 0x00, 0x81,  // Row 2: edges only
    // ... more rows ...
};

void example_ui_bitmap(void) {
    bitmap_t button = bitmap_create(
        button_bitmap,
        sizeof(button_bitmap),
        32, 16,
        BITMAP_FORMAT_MONO
    );
    
    // Use in UI rendering function
    // render_bitmap(&button, x, y);
}

