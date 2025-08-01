# Makefile for ARK AArch64 kernel on RPi Zero 2 W

# Toolchain (make sure it's in your PATH or specify full path)
CROSS_COMPILE = aarch64-none-elf-
CC = $(CROSS_COMPILE)gcc
AS = $(CROSS_COMPILE)as
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

# Source files
C_SRCS = src/drivers/uart.c src/kernel/main.c
ASM_SRCS = src/arch/arm/boot/entry.S

# Object files
C_OBJS = $(C_SRCS:.c=.o)
ASM_OBJS = $(ASM_SRCS:.S=.o)
OBJS = $(ASM_OBJS) $(C_OBJS)

# Output files
KERNEL_ELF = ark.elf
KERNEL_IMG = kernel8.img # MUST be named kernel8.img for AArch64 on RPi

# Linker script
LDSCRIPT = scripts/linker.ld

# Compiler flags
# -Wall: Enable all warnings
# -O2: Optimization level 2
# -ffreestanding: No standard library environment (important for bare-metal)
# -nostdlib: Don't link standard libraries
# -nostdinc: Don't search standard system include directories
# -Iinclude: Add our custom include directory
# -mcpu=cortex-a53: Target Cortex-A53 (RPi Zero 2 W CPU)
# -gdwarf-2: Generate debug information
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostdinc -Iinclude -mcpu=cortex-a53 -gdwarf-2
AFLAGS = -mcpu=cortex-a53 -g # -g for debugging info in assembly

# Linker flags
LDFLAGS = -nostdlib -T $(LDSCRIPT)

.PHONY: all clean flash qemu

all: $(KERNEL_IMG)

$(KERNEL_IMG): $(KERNEL_ELF)
	$(OBJCOPY) -O binary $< $@

$(KERNEL_ELF): $(OBJS) $(LDSCRIPT)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

# C source compilation
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Assembly source compilation
%.o: %.S
	$(CC) $(AFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(KERNEL_ELF) $(KERNEL_IMG)

flash: all
	@echo "--------------------------------------------------------"
	@echo "IMPORTANT: Copy $(KERNEL_IMG) to the FAT32 boot partition"
	@echo "of your RPi Zero 2 W SD card."
	@echo "Also ensure 'arm_64bit=1' is in config.txt on the SD card."
	@echo "Connect your USB-to-TTL serial adapter to GPIO14/15."
	@echo "Then power on your Raspberry Pi Zero 2 W."
	@echo "--------------------------------------------------------"

qemu: $(KERNEL_IMG)
	@echo "Running on QEMU (emulating Raspberry Pi 3)..."
	qemu-system-aarch64 -M raspi3 -cpu cortex-a53 -kernel $(KERNEL_IMG) -serial stdio -d guest_errors -no-reboot
