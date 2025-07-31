CC = gcc
LD = ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
AFLAGS = --32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

C_SOURCES = src/root/kernel.c src/sh/shell.c src/crp/string.c src/driver/keyboard.c src/crp/ata.c src/fs/fat32.c src/uilib/ui.c src/sh/cd.c src/sh/exit.c src/sh/ls.c src/sh/lfetch.c src/sh/ltime.c src/sh/mkdir.c src/sh/rmdir.c src/sh/echo.c src/fs/btfs.c src/fs/fat12.c src/fs/lfs.c src/fs/nfs.c src/fs/lfs.c

OBJS = $(C_SOURCES:.c=.o)

all: initdisk.img

initdisk.img: $(OBJS)
	@echo "==>     $@"
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	@echo "==>     $<"
	@$(CC) $(CFLAGS) -c $< -o $@


run: build
	@echo "Running in QEMU..."
	qemu-system-i386 -cdrom image/ArcOS-Salvador1.4k.iso -m 512M -boot d

build: initdisk.img
	@echo "Building ISO..."
	mkdir -p image/boot/grub
	cp initdisk.bin image/boot/
	cp boot/grub.cfg image/boot/grub/grub.cfg
	grub-mkrescue -o image/ArcOS-Salvador1.4k.iso image > /dev/null 2>&1

clean:
	@echo "Cleaning up..."
	find src/ -name '*.o' -type f -delete
	rm -f *.o initdisk.img
	rm -rf image

