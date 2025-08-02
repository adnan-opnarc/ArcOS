ENTRY(kernel_main)

SECTIONS {
  . = 0x100000;

  .multiboot : {
    *(.multiboot)
  } :text

  .text : {
    *(.text*)
  } :text

  .rodata : {
    *(.rodata*)
  } :rodata

  .data : {
    *(.data*)
  } :data

  .bss : {
    *(.bss*)
    *(COMMON)
  } :data
}

PHDRS {
  text PT_LOAD FLAGS(4);     /* R | X */
  rodata PT_LOAD FLAGS(4);   /* R     */
  data PT_LOAD FLAGS(6);     /* R | W */
}

