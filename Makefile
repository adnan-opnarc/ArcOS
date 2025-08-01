# Define default ARCH if not specified on command line.
# This must be at the top level to affect variable definitions.
arch ?= i386

ifeq ($(arch),i386)
	CC = gcc
	LD = ld
	CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
	AFLAGS = --32
	LDFLAGS = -T linker.ld -nostdlib -m elf_i386
	GRUB_CFG = boot/grub.cfg
	QEMU_CMD = qemu-system-i386 -cdrom image/ArcOS-Salvador1.4k_$(arch).iso -m 512M -boot d
	C_SOURCES = \
		src/root/x86/kernel.c \
		src/sh/x86/shell.c \
		src/crp/x86/string.c \
		src/driver/x86/keyboard.c \
		src/crp/x86/ata.c \
		src/fs/fat32.c \
		src/uilib/x86/ui.c \
		src/sh/x86/cd.c \
		src/sh/x86/exit.c \
		src/sh/x86/ls.c \
		src/sh/x86/lfetch.c \
		src/sh/x86/ltime.c \
		src/sh/x86/mkdir.c \
		src/sh/x86/rmdir.c \
		src/sh/x86/echo.c \
		src/fs/btfs.c \
		src/fs/fat12.c \
		src/fs/lfs.c \
		src/fs/nfs.c
else ifeq ($(arch),arm64)
	CC = aarch64-linux-gnu-gcc
	LD = aarch64-linux-gnu-ld
	CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -march=armv8-a -fno-stack-protector -Isrc
	LDFLAGS = -T linker_arm64.ld -nostdlib
	GRUB_CFG = boot/grubarch64.cfg
	QEMU_CMD = qemu-system-aarch64 -M virt -cpu cortex-a57 -kernel image/initdisk_$(arch).img -m 1024M -append "root=/dev/vda console=ttyAMA0" -nographic
	C_SOURCES = \
		src/root/arm/kernel.c \
		src/sh/arm/shell.c \
		src/crp/arm/string.c \
		src/driver/arm/keyboard.c \
		src/driver/arm/uart.c \
		src/crp/arm/ata.c \
		src/fs/fat32.c \
		src/uilib/arm/ui.c \
		src/sh/arm/cd.c \
		src/sh/arm/exit.c \
		src/sh/arm/ls.c \
		src/sh/arm/lfetch.c \
		src/sh/arm/ltime.c \
		src/sh/arm/mkdir.c \
		src/sh/arm/rmdir.c \
		src/sh/arm/echo.c \
		src/fs/btfs.c \
		src/fs/fat12.c \
		src/fs/lfs.c \
		src/fs/nfs.c
else
	$(error Invalid architecture specified: $(arch). Must be i386 or arm64.)
endif

KERNEL_OBJS = $(C_SOURCES:.c=.o)
KERNEL_IMAGE = initdisk_$(arch).img
ISO_NAME = ArcOS-Salvador1.4k_$(arch).iso

all: $(KERNEL_IMAGE)

$(KERNEL_IMAGE): $(KERNEL_OBJS)
	@echo "==>ldk       $@"
	$(LD) $(LDFLAGS) -o $@ $(KERNEL_OBJS)

%.o: %.c
	@mkdir -p $(@D)
	@echo "==>gcc       $<"
	$(CC) $(CFLAGS) -c $< -o $@

build: $(arch)

$(arch): $(KERNEL_IMAGE)
	@echo "Building image for $(arch)..."
	@mkdir -p image/boot/grub
	@cp $(KERNEL_IMAGE) image/boot/
ifeq ($(arch),i386)
	@cp $(GRUB_CFG) image/boot/grub/grub.cfg
	@grub-mkrescue -o image/$(ISO_NAME) image
else ifeq ($(arch),arm64)
	@echo "Skipping grub-mkrescue for ARM64. QEMU will boot directly from kernel image."
endif

run: build
	@echo "Running in QEMU (Architecture: $(arch))..."
	$(QEMU_CMD)

clean:
	@echo "Cleaning up..."
	@find src/ -name '*.o' -type f -delete
	@rm -f $(KERNEL_IMAGE) image/$(ISO_NAME)
	@rm -rf image
	@echo "Cleanup complete."

help:
	@echo ""
	@echo "---------------------------------------"
	@echo "                  x86                  "
	@echo ""
	@echo "+build for i386"
	@echo "make build arch=i386"
	@echo "+only initdisk"
	@echo "make arch=i386"
	@echo "+to build and run(preffered)"
	@echo "make run arch=i386"
	@echo ""
	@echo "---------------------------------------"
	@echo "                  arm                  "
	@echo ""
	@echo "+build for arm(new doesnt work for now)"
	@echo "make build arch=arm64"
	@echo "+only initdisk"
	@echo "make arch=arm64"
	@echo "+to build and run(preffered)"
	@echo "make run arch=arm64"
	
	
	
.PHONY: all build run clean $(arch)

