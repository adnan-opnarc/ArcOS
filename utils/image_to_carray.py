#!/usr/bin/env python3
"""
Convert image file to C array bitmap
Supports: Raw binary, indexed, RGB, RGBA formats

Usage: image_to_carray.py <input.bin> <output.h> <width> <height> <format> [array_name]
Formats: mono, indexed, raw, rgb, rgba
"""

import sys
import os

def image_to_carray(input_file, output_file, width, height, format_type, array_name="bitmap_data"):
    """Convert image file to C array header file"""
    
    if not os.path.exists(input_file):
        print(f"Error: Input file '{input_file}' not found", file=sys.stderr)
        return 1
    
    with open(input_file, 'rb') as f:
        data = f.read()
    
    # Calculate expected size
    format_map = {
        'mono': (width * height + 7) // 8,
        'indexed': width * height,
        'raw': width * height,
        'rgb': width * height * 3,
        'rgba': width * height * 4
    }
    
    if format_type not in format_map:
        print(f"Error: Unknown format '{format_type}'", file=sys.stderr)
        print(f"Supported: {', '.join(format_map.keys())}", file=sys.stderr)
        return 1
    
    expected_size = format_map[format_type]
    if len(data) < expected_size:
        print(f"Warning: File size {len(data)} < expected {expected_size}", file=sys.stderr)
    
    size = len(data)
    header_guard = f"BITMAP_{array_name.upper()}_H"
    
    with open(output_file, 'w') as f:
        f.write(f"#ifndef {header_guard}\n")
        f.write(f"#define {header_guard}\n\n")
        f.write(f"// Auto-generated from {os.path.basename(input_file)}\n")
        f.write(f"// Bitmap: {width}x{height}, Format: {format_type}\n")
        f.write(f"// Size: {size} bytes\n\n")
        f.write(f"#include <stdint.h>\n\n")
        f.write(f"#define {array_name.upper()}_WIDTH {width}\n")
        f.write(f"#define {array_name.upper()}_HEIGHT {height}\n")
        f.write(f"#define {array_name.upper()}_SIZE {size}\n")
        f.write(f"#define {array_name.upper()}_FORMAT BITMAP_FORMAT_{format_type.upper()}\n\n")
        f.write(f"static const uint8_t {array_name}[] = {{\n")
        
        # Write data in hex format, 16 bytes per line
        for i in range(0, size, 16):
            line = "    "
            for j in range(16):
                if i + j < size:
                    line += f"0x{data[i + j]:02X}, "
                else:
                    line += "     "
            f.write(line.rstrip() + "\n")
        
        f.write("};\n\n")
        f.write(f"#endif // {header_guard}\n")
    
    print(f"Successfully converted {input_file} to {output_file}")
    print(f"  Array name: {array_name}")
    print(f"  Dimensions: {width}x{height}")
    print(f"  Format: {format_type}")
    print(f"  Size: {size} bytes")
    return 0

if __name__ == "__main__":
    if len(sys.argv) < 6:
        print("Usage: image_to_carray.py <input.bin> <output.h> <width> <height> <format> [array_name]", file=sys.stderr)
        print("Formats: mono, indexed, raw, rgb, rgba", file=sys.stderr)
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    width = int(sys.argv[3])
    height = int(sys.argv[4])
    format_type = sys.argv[5].lower()
    array_name = sys.argv[6] if len(sys.argv) > 6 else "bitmap_data"
    
    sys.exit(image_to_carray(input_file, output_file, width, height, format_type, array_name))

