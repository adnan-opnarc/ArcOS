/* scripts/ark.ld */

ENTRY(_start)

SECTIONS
{
    /* The AArch64 kernel will be loaded by the GPU at 0x80000 */
    . = 0x80000;

    .text :
    {
        KEEP(*(.text.boot)) /* Keep our entry point at the very beginning */
        *(.text)            /* All other code */
    }

    .rodata :
    {
        *(.rodata)          /* Read-only data */
    }

    .data :
    {
        *(.data)            /* Initialized data */
    }

    .bss :
    {
        _bss_start = .;     /* Start of BSS (uninitialized data) */
        *(.bss)             /* Uninitialized data */
        _bss_end = .;       /* End of BSS */
    }

    /DISCARD/ : /* Discard unused sections */
    {
        *(.comment)
        *(.ARM.exidx)
    }
}
