# =========================
# ArcOS Build System
# =========================
VER  := 1.7
CN   := archaeono
# ... (rest of your variables)

CC              := gcc
LD              := ld
OBJCOPY         := objcopy
GRUB_MKRESCUE   := grub-mkrescue
QEMU            := qemu-system-i386

CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
LDFLAGS := -T binlink.ld -nostdlib -m elf_i386

KERNEL_IMG := ArcOS-i386.img
ISO_DIR    := iso_root
ISO_IMAGE  := image/ArcOS-$(CN)$(VER)k_i386.iso

# FONT CONFIGURATION
FONT_PSF := src/x86/fonts/cp850-8x16.psf
FONT_OBJ := src/x86/root/font_data.o
FONT_ARRAY_H := src/x86/root/font_array.h

SRC := \
    src/x86/root/kernel.c \
    src/x86/sh/shell.c \
    src/x86/driver/keyboard.c \
    src/x86/root/font.c \
    src/x86/crp/string.c \
    src/x86/crp/bitmap.c \
    src/x86/fs/ramfs.c \
    src/x86/uilib/ui.c \
    src/x86/sh/pid.c \
    src/x86/sh/exit.c \
    src/x86/sh/ls.c \
    src/x86/sh/lfetch.c \
    src/x86/sh/ltime.c \
    src/x86/sh/mkdir.c \
    src/x86/sh/rmdir.c \
    src/x86/sh/echo.c \
    src/x86/sh/man.c \
    src/x86/sh/bitmap.c \
    src/x86/sh/cat.c \
    src/x86/sh/touch.c \
    src/x86/sh/script.c \
    src/x86/cr/print.c\
    src/x86/cr/input.c \
    src/x86/cr/ccrp.c   \
    src/x86/root/tui.c

# Combine C objects and the binary font object
OBJ := $(SRC:.c=.o) $(FONT_OBJ)

# TARGETS
all: build iso

build: kernel

# Generate C array from PSF (optional, for C array font support)
$(FONT_ARRAY_H): $(FONT_PSF)
	@mkdir -p src/x86/fonts
	@echo -e "$(YELLOW) => Generating C array: $@$(RESET)"
	@python3 utils/psf_to_carray.py $< $@ font_array

$(FONT_OBJ): $(FONT_PSF)
	@mkdir -p src/x86/fonts
	@echo -e "$(YELLOW) => Embedding Font: $<$(RESET)"
	@$(OBJCOPY) -I binary -O elf32-i386 -B i386 $< $@
	@# This next line renames the messy symbol to something clean
	@objcopy --redefine-sym $$(nm $@ | grep _start | cut -d ' ' -f 3)=_font_start $@
	
%.o: %.c
	@$(CC) $(CFLAGS) -c -o $@ $<

kernel: $(OBJ)
	@$(LD) $(LDFLAGS) -o $(KERNEL_IMG) $^

iso: kernel
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL_IMG) $(ISO_DIR)/boot/
	@cp boot/grub.cfg $(ISO_DIR)/boot/grub/
	@mkdir -p image
	@$(GRUB_MKRESCUE) -o $(ISO_IMAGE) $(ISO_DIR)

run: iso
	@$(QEMU) -cdrom $(ISO_IMAGE) -m 6m

clean:
	@rm -rf $(OBJ) $(KERNEL_IMG) $(ISO_DIR) image/*.iso
