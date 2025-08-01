ENTRY(_start)

SECTIONS {
    . = 0x80000; /* Common load address for basic ARM64 kernels,
                    e.g., on QEMU 'virt' machine.
                    Alternatively, 0x40000000 (1GB) is also very common. */

    .text : {
        *(.text)
        *(.text.*)
    }

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }

    .data : {
        *(.data)
        *(.data.*)
    }

    .bss : {
        *(.bss)
        *(.bss.*)
    }

    /DISCARD/ : { /* Discard sections not usually needed in a freestanding kernel */
        *(.eh_frame)
        *(.eh_frame_hdr)
        *(.comment)
        *(.ARM.exidx)
    }
}
