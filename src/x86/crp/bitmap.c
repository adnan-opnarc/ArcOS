#include "bitmap.h"

// Calculate expected data size for format
size_t bitmap_calc_size(uint32_t width, uint32_t height, bitmap_format_t format) {
    switch (format) {
        case BITMAP_FORMAT_MONO:
            return (width * height + 7) / 8;  // 1 bit per pixel
        case BITMAP_FORMAT_INDEXED:
            return width * height;  // 1 byte per pixel
        case BITMAP_FORMAT_RAW:
            return width * height;  // 1 byte per pixel (raw)
        case BITMAP_FORMAT_RGB:
            return width * height * 3;  // 3 bytes per pixel
        case BITMAP_FORMAT_RGBA:
            return width * height * 4;  // 4 bytes per pixel
        default:
            return 0;
    }
}

// Create bitmap from C array
bitmap_t bitmap_create(const uint8_t* data, size_t data_size,
                       uint32_t width, uint32_t height,
                       bitmap_format_t format) {
    bitmap_t bmp = {0};
    
    if (!data) return bmp;
    
    size_t expected_size = bitmap_calc_size(width, height, format);
    if (data_size < expected_size) return bmp;
    
    bmp.width = width;
    bmp.height = height;
    bmp.format = format;
    bmp.data = data;
    bmp.data_size = data_size;
    bmp.palette = NULL;
    bmp.palette_size = 0;
    
    return bmp;
}

// Create indexed bitmap with palette
bitmap_t bitmap_create_indexed(const uint8_t* data, size_t data_size,
                                uint32_t width, uint32_t height,
                                const uint8_t* palette, uint32_t palette_size) {
    bitmap_t bmp = bitmap_create(data, data_size, width, height, BITMAP_FORMAT_INDEXED);
    bmp.palette = palette;
    bmp.palette_size = palette_size;
    return bmp;
}

// Validate bitmap
bool bitmap_is_valid(const bitmap_t* bmp) {
    if (!bmp || !bmp->data) return false;
    if (bmp->width == 0 || bmp->height == 0) return false;
    
    size_t expected_size = bitmap_calc_size(bmp->width, bmp->height, bmp->format);
    if (bmp->data_size < expected_size) return false;
    
    if (bmp->format == BITMAP_FORMAT_INDEXED && bmp->palette_size == 0) return false;
    
    return true;
}

// Get pixel value at (x, y) - returns raw byte value
uint8_t bitmap_get_pixel_raw(const bitmap_t* bmp, uint32_t x, uint32_t y) {
    if (!bitmap_is_valid(bmp)) return 0;
    if (x >= bmp->width || y >= bmp->height) return 0;
    
    uint32_t offset = 0;
    
    switch (bmp->format) {
        case BITMAP_FORMAT_MONO: {
            uint32_t bit_index = y * bmp->width + x;
            uint32_t byte_index = bit_index / 8;
            uint8_t bit_pos = 7 - (bit_index % 8);
            return (bmp->data[byte_index] >> bit_pos) & 1;
        }
        case BITMAP_FORMAT_INDEXED:
        case BITMAP_FORMAT_RAW:
            offset = y * bmp->width + x;
            return bmp->data[offset];
        case BITMAP_FORMAT_RGB:
            offset = (y * bmp->width + x) * 3;
            return bmp->data[offset];  // Returns R component
        case BITMAP_FORMAT_RGBA:
            offset = (y * bmp->width + x) * 4;
            return bmp->data[offset];  // Returns R component
        default:
            return 0;
    }
}

// Get pixel as RGB (for indexed/mono formats, uses palette)
void bitmap_get_pixel_rgb(const bitmap_t* bmp, uint32_t x, uint32_t y,
                           uint8_t* r, uint8_t* g, uint8_t* b) {
    if (!bitmap_is_valid(bmp) || !r || !g || !b) {
        *r = *g = *b = 0;
        return;
    }
    
    if (x >= bmp->width || y >= bmp->height) {
        *r = *g = *b = 0;
        return;
    }
    
    uint32_t offset = 0;
    
    switch (bmp->format) {
        case BITMAP_FORMAT_MONO: {
            uint8_t bit = bitmap_get_pixel_raw(bmp, x, y);
            if (bmp->palette && bmp->palette_size > 0) {
                uint32_t palette_idx = bit * 3;
                *r = bmp->palette[palette_idx];
                *g = bmp->palette[palette_idx + 1];
                *b = bmp->palette[palette_idx + 2];
            } else {
                // Default: black or white
                *r = *g = *b = bit ? 255 : 0;
            }
            break;
        }
        case BITMAP_FORMAT_INDEXED: {
            uint8_t idx = bitmap_get_pixel_raw(bmp, x, y);
            if (bmp->palette && idx < bmp->palette_size) {
                uint32_t palette_idx = idx * 3;
                *r = bmp->palette[palette_idx];
                *g = bmp->palette[palette_idx + 1];
                *b = bmp->palette[palette_idx + 2];
            } else {
                *r = *g = *b = 0;
            }
            break;
        }
        case BITMAP_FORMAT_RAW: {
            uint8_t val = bitmap_get_pixel_raw(bmp, x, y);
            *r = *g = *b = val;  // Grayscale
            break;
        }
        case BITMAP_FORMAT_RGB: {
            offset = (y * bmp->width + x) * 3;
            *r = bmp->data[offset];
            *g = bmp->data[offset + 1];
            *b = bmp->data[offset + 2];
            break;
        }
        case BITMAP_FORMAT_RGBA: {
            offset = (y * bmp->width + x) * 4;
            *r = bmp->data[offset];
            *g = bmp->data[offset + 1];
            *b = bmp->data[offset + 2];
            // Alpha is at offset + 3, but we ignore it here
            break;
        }
        default:
            *r = *g = *b = 0;
            break;
    }
}

