#ifndef BITMAP_CMD_H
#define BITMAP_CMD_H

#include <stdint.h>

// Draw bitmap at specific position (for use in code)
void draw_bitmap_at(int x, int y, const uint8_t* data, uint32_t width, uint32_t height);

// Bitmap command (parses command string)
void bitmap_main(const char* args);

#endif

