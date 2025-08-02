#code name
VER = 1.5
CN = opassium
SDD = kvf
MM = 4.8
BUID = 174893KJF0098

# Compiler and tools
CC := gcc
LD := ld
GRUB_MKRESCUE := grub-mkrescue
QEMU := qemu-system-i386

# Flags
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
LDFLAGS := -T binlink.ld -nostdlib -m elf_i386

# Output
KERNEL_IMG := ArcOS-i386.img
ISO_DIR := iso_root
ISO_IMAGE := image/ArcOS-Salvador1.5k_i386.iso

# Source and object files
SRC := \
	src/x86/root/x86/kernel.c \
	src/x86/sh/x86/shell.c \
	src/x86/crp/x86/string.c \
	src/x86/driver/x86/keyboard.c \
	src/x86/crp/x86/ata.c \
	src/x86/fs/fat32.c \
	src/x86/uilib/x86/ui.c \
	src/x86/sh/x86/cd.c \
	src/x86/sh/x86/exit.c \
	src/x86/sh/x86/ls.c \
	src/x86/sh/x86/lfetch.c \
	src/x86/sh/x86/ltime.c \
	src/x86/sh/x86/mkdir.c \
	src/x86/sh/x86/rmdir.c \
	src/x86/sh/x86/echo.c \
	src/x86/fs/btfs.c \
	src/x86/fs/fat12.c \
	src/x86/fs/lfs.c \
	src/x86/fs/nfs.c

OBJ := $(SRC:.c=.o)

# Colors
BLUE := \033[1;34m
GREEN := \033[1;32m
RESET := \033[0m

.PHONY: all build kernel iso clean run

all: build iso

build: kernel

# Compile each .c to .o with colored messages
%.o: %.c
	@echo -e "$(BLUE) => gcc $<$(RESET)"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Link all objects into a kernel image with colored message
kernel: $(OBJ)
	@echo -e "$(GREEN)==> Linking x86 kernel: $(KERNEL_IMG)$(RESET)"
	@$(LD) $(LDFLAGS) -o $(KERNEL_IMG) $^

iso: kernel
	@echo -e "$(GREEN) => Preparing ISO directory$(RESET)"
	@rm -rf $(ISO_DIR)
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL_IMG) $(ISO_DIR)/boot/$(KERNEL_IMG)
	@cp boot/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg

	@echo -e "$(GREEN) => Creating bootable ISO with grub-mkrescue$(RESET)"
	@mkdir -p image
	@$(GRUB_MKRESCUE) -o $(ISO_IMAGE) $(ISO_DIR)
	@echo -e "$(GREEN)ISO image created at $(ISO_IMAGE)$(RESET)"

run: iso
	@echo -e "$(GREEN) => Running ISO in QEMU$(RESET)"
	@$(QEMU) -cdrom $(ISO_IMAGE) -m 512

clean:
	@echo -e "$(BLUE) => Cleaning build artifacts$(RESET)"
	@rm -rf $(OBJ) $(KERNEL_IMG) $(ISO_DIR) image/*.iso

