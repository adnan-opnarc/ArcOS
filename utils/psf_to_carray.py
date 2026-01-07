#!/usr/bin/env python3
"""
Convert PSF font file to C array format
Usage: psf_to_carray.py <input.psf> <output.h> [array_name]
"""

import sys
import os

def psf_to_carray(input_file, output_file, array_name="font_data"):
    """Convert PSF file to C array header file"""
    
    if not os.path.exists(input_file):
        print(f"Error: Input file '{input_file}' not found", file=sys.stderr)
        return 1
    
    with open(input_file, 'rb') as f:
        data = f.read()
    
    if len(data) < 4:
        print("Error: File too small to be a valid PSF", file=sys.stderr)
        return 1
    
    # Check PSF1 magic (0x0436 in little-endian)
    magic = int.from_bytes(data[0:2], 'little')
    if magic != 0x0436:
        print(f"Warning: Magic number 0x{magic:04X} doesn't match PSF1 (0x0436)", file=sys.stderr)
    
    size = len(data)
    header_guard = f"FONT_{array_name.upper()}_H"
    
    with open(output_file, 'w') as f:
        f.write(f"#ifndef {header_guard}\n")
        f.write(f"#define {header_guard}\n\n")
        f.write(f"// Auto-generated from {os.path.basename(input_file)}\n")
        f.write(f"// Font size: {size} bytes\n\n")
        f.write(f"#include <stdint.h>\n\n")
        f.write(f"#define {array_name.upper()}_SIZE {size}\n\n")
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
        f.write(f"// Glyphs start at offset {4} (after PSF header)\n")
        f.write(f"#define {array_name.upper()}_GLYPHS ({array_name} + 4)\n")
        f.write(f"#define {array_name.upper()}_GLYPH_COUNT 256\n")
        f.write(f"#define {array_name.upper()}_GLYPH_SIZE 16\n\n")
        f.write(f"#endif // {header_guard}\n")
    
    print(f"Successfully converted {input_file} to {output_file}")
    print(f"  Array name: {array_name}")
    print(f"  Size: {size} bytes")
    return 0

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: psf_to_carray.py <input.psf> <output.h> [array_name]", file=sys.stderr)
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    array_name = sys.argv[3] if len(sys.argv) > 3 else "font_data"
    
    sys.exit(psf_to_carray(input_file, output_file, array_name))

