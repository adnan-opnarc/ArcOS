#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Bitmap format types
typedef enum {
    BITMAP_FORMAT_RAW,      // Raw pixel data
    BITMAP_FORMAT_INDEXED,  // Indexed color palette
    BITMAP_FORMAT_MONO,     // Monochrome (1 bit per pixel)
    BITMAP_FORMAT_RGB,      // RGB (24-bit)
    BITMAP_FORMAT_RGBA      // RGBA (32-bit)
} bitmap_format_t;

// Bitmap structure
typedef struct {
    uint32_t width;
    uint32_t height;
    bitmap_format_t format;
    const uint8_t* data;     // Pointer to C array data
    size_t data_size;        // Size of data array in bytes
    const uint8_t* palette;  // Optional palette for indexed format
    uint32_t palette_size;   // Number of palette entries
} bitmap_t;

// Create bitmap from C array
bitmap_t bitmap_create(const uint8_t* data, size_t data_size, 
                       uint32_t width, uint32_t height, 
                       bitmap_format_t format);

// Create indexed bitmap with palette
bitmap_t bitmap_create_indexed(const uint8_t* data, size_t data_size,
                                uint32_t width, uint32_t height,
                                const uint8_t* palette, uint32_t palette_size);

// Get pixel value at (x, y) - returns raw byte value
uint8_t bitmap_get_pixel_raw(const bitmap_t* bmp, uint32_t x, uint32_t y);

// Get pixel as RGB (for indexed/mono formats, uses palette)
void bitmap_get_pixel_rgb(const bitmap_t* bmp, uint32_t x, uint32_t y,
                           uint8_t* r, uint8_t* g, uint8_t* b);

// Validate bitmap
bool bitmap_is_valid(const bitmap_t* bmp);

// Calculate expected data size for format
size_t bitmap_calc_size(uint32_t width, uint32_t height, bitmap_format_t format);

#endif // BITMAP_H

