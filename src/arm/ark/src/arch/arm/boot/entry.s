/* src/arch/arm/boot/entry.S */

.section ".text.boot"

.global _start
_start:
    // Set up the stack pointer (SP_EL1)
    // For simplicity, we'll put the stack at the end of some assumed memory,
    // far from where our code is, or just above the kernel's load address.
    // A more robust kernel would set up a proper memory map and stack.
    // Let's assume a temporary stack somewhat above the kernel load address.
    // If our kernel is small, we can place the stack below the typical 1GB boundary for now.
    // For RPi Zero 2 W with 512MB RAM, an address like 0x8000000 or 0x20000000 could be used
    // as a temporary stack, but 0x80000 + some offset is safer to avoid clashing with peripheral memory.
    // Let's use 0x80000 + 0x100000 (1MB) as a temporary stack top.
    mov     x0, #0x80000        // Kernel base address
    add     x0, x0, #0x100000   // Add 1MB offset for stack
    mov     sp, x0              // Set SP_EL1 (kernel stack pointer)

    // Clear the BSS segment
    ldr     x0, =_bss_start     // Get start of BSS
    ldr     x1, =_bss_end       // Get end of BSS
    sub     x1, x1, x0          // Calculate size of BSS
    cmp     x1, #0              // Check if BSS has size
    beq     2f                  // If size is 0, skip
1:
    str     xzr, [x0], #8       // Store 0 to current 8-byte word, increment address
    subs    x1, x1, #8          // Decrement remaining size
    bgt     1b                  // Loop if size > 0
2:

    // Branch to C kernel entry point
    bl      kernel_main

    // If kernel_main returns, halt the CPU (or loop infinitely)
    b       .                   // Infinite loop
