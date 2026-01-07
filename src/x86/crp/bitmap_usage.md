# Bitmap C Array Support

This system allows you to use custom bitmaps as C arrays in any system function.

## Quick Start

### 1. Define Your Bitmap as C Array

```c
#include "bitmap.h"

// Example: 8x8 monochrome icon
static const uint8_t my_icon[] = {
    0b00011000,  // Row 0
    0b00111100,  // Row 1
    0b01111110,  // Row 2
    0b11111111,  // Row 3
    0b11111111,  // Row 4
    0b01111110,  // Row 5
    0b00111100,  // Row 6
    0b00011000   // Row 7
};

// Create bitmap
bitmap_t icon = bitmap_create(
    my_icon,
    sizeof(my_icon),
    8, 8,
    BITMAP_FORMAT_MONO
);
```

### 2. Use in Any System Function

```c
void my_system_function(void) {
    bitmap_t icon = bitmap_create(my_icon, sizeof(my_icon), 8, 8, BITMAP_FORMAT_MONO);
    
    // Access pixels
    uint8_t pixel = bitmap_get_pixel_raw(&icon, 3, 3);
    
    // Get RGB values
    uint8_t r, g, b;
    bitmap_get_pixel_rgb(&icon, 3, 3, &r, &g, &b);
}
```

## Supported Formats

### BITMAP_FORMAT_MONO
- 1 bit per pixel
- Size: `(width * height + 7) / 8` bytes
- Use for: Icons, simple graphics

### BITMAP_FORMAT_INDEXED
- 1 byte per pixel (palette index)
- Requires palette array
- Size: `width * height` bytes
- Use for: Sprites, UI elements

### BITMAP_FORMAT_RAW
- 1 byte per pixel (grayscale)
- Size: `width * height` bytes
- Use for: Grayscale images

### BITMAP_FORMAT_RGB
- 3 bytes per pixel (R, G, B)
- Size: `width * height * 3` bytes
- Use for: Full color images

### BITMAP_FORMAT_RGBA
- 4 bytes per pixel (R, G, B, A)
- Size: `width * height * 4` bytes
- Use for: Images with transparency

## Examples

### Monochrome Bitmap
```c
static const uint8_t icon[] = { /* ... */ };
bitmap_t bmp = bitmap_create(icon, sizeof(icon), 8, 8, BITMAP_FORMAT_MONO);
```

### Indexed Bitmap with Palette
```c
static const uint8_t sprite[] = { /* 256 bytes */ };
static const uint8_t palette[] = {
    0x00, 0x00, 0x00,  // Color 0: Black
    0xFF, 0x00, 0x00,  // Color 1: Red
    0x00, 0xFF, 0x00,  // Color 2: Green
};
bitmap_t bmp = bitmap_create_indexed(sprite, sizeof(sprite), 16, 16, palette, 3);
```

### RGB Bitmap
```c
static const uint8_t image[] = {
    0xFF, 0x00, 0x00,  // Pixel 0: Red
    0x00, 0xFF, 0x00,  // Pixel 1: Green
    // ...
};
bitmap_t bmp = bitmap_create(image, sizeof(image), 10, 10, BITMAP_FORMAT_RGB);
```

## API Functions

- `bitmap_create()` - Create bitmap from C array
- `bitmap_create_indexed()` - Create indexed bitmap with palette
- `bitmap_is_valid()` - Validate bitmap
- `bitmap_get_pixel_raw()` - Get raw pixel value
- `bitmap_get_pixel_rgb()` - Get pixel as RGB
- `bitmap_calc_size()` - Calculate expected size

## Converting Images to C Arrays

Use the utility script:
```bash
python3 utils/image_to_carray.py input.bin output.h 32 32 indexed my_bitmap
```

This generates a header file with your bitmap as a C array ready to use!

