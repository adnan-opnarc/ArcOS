# =========================
# No More UPDATES.closed
# =========================
# =========================
# ArcOS Build System
# =========================
VER  := 1.7
CN   := archaeono
SDD  := kvf
MM   := 5.2
BUID := 174893KJF0098
rev  := 1

# =========================
# Compiler and Tools
# =========================
CC              := gcc
LD              := ld
GRUB_MKRESCUE   := grub-mkrescue
QEMU            := qemu-system-i386
GIT             := git clone
REPO_URL        := https://github.com/adnan-opnarc/ArcOS.git
BACKUP_DIR      := backup

# =========================
# Build Flags
# =========================
CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
LDFLAGS := -T binlink.ld -nostdlib -m elf_i386

# =========================
# Output
# =========================
KERNEL_IMG := ArcOS-i386.img
ISO_DIR    := iso_root
ISO_IMAGE  := image/ArcOS-$(CN)$(VER)k_i386.iso

# =========================
# Sources and Objects
# =========================
SRC := \
	src/x86/root/kernel.c \
	src/x86/root/kernel_panic.c \
	src/x86/sh/shell.c \
	src/x86/crp/string.c \
	src/x86/driver/keyboard.c \
	src/x86/crp/ata.c \
	src/x86/fs/fat32.c \
	src/x86/uilib/ui.c \
	src/x86/sh/cd.c \
	src/x86/sh/exit.c \
	src/x86/sh/ls.c \
	src/x86/sh/lfetch.c \
	src/x86/sh/ltime.c \
	src/x86/sh/mkdir.c \
	src/x86/sh/rmdir.c \
	src/x86/sh/echo.c \
	src/x86/fs/btfs.c \
	src/x86/fs/fat12.c \
	src/x86/fs/lfs.c \
	src/x86/fs/nfs.c \
	src/x86/cr/ccrp.c \
	src/x86/cr/input.c \
	src/x86/cr/print.c \

OBJ := $(SRC:.c=.o)

# =========================
# Terminal Colors
# =========================
BLUE   := \033[1;34m
GREEN  := \033[1;32m
YELLOW := \033[1;33m
RESET  := \033[0m

# =========================
# Parallelism
# =========================
JOBS ?= 4

# =========================
# Targets
# =========================
.PHONY: all build kernel iso clean run restore fast

all: build iso
ifeq ($(MAKELEVEL),0)
  ifeq ($(filter -j,$(MAKEFLAGS)),)
	@echo -e "$(YELLOW)Tip: Use 'make -j$(JOBS)' to build using multiple cores.$(RESET)"
  endif
endif

build: kernel

fast:
	@echo -e "$(GREEN)Building with $(JOBS) cores...$(RESET)"
	@$(MAKE) -j$(JOBS) all

%.o: %.c
	@echo -e "$(BLUE) => Compiling: $<$(RESET)"
	@$(CC) $(CFLAGS) -c -o $@ $<

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
	@echo -e "$(GREEN)Running ISO in QEMU...$(RESET)"
	@$(QEMU) -cdrom $(ISO_IMAGE) -m 512

clean:
	@echo -e "$(BLUE)Cleaning build artifacts$(RESET)"
	@rm -rf $(OBJ) $(KERNEL_IMG) $(ISO_DIR) image/*.iso

restore:
	@echo -e "$(BLUE)Restoring files from $(REPO_URL) into $(BACKUP_DIR)$(RESET)"
	@mkdir -p $(BACKUP_DIR)
	@cd $(BACKUP_DIR) && $(GIT) $(REPO_URL)
	@echo -e "$(GREEN)Repository cloned into $(BACKUP_DIR)$(RESET)"
