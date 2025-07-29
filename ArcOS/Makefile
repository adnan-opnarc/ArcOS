AS = as
CC = gcc
LD = ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -fno-stack-protector -Isrc
AFLAGS = --32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

C_SOURCES = src/kernel.c src/shell.c src/string.c src/keyboard.c src/ata.c src/fat32.c src/ui.c
S_SOURCES = $(wildcard src/*.s)
OBJS = $(C_SOURCES:.c=.o) $(S_SOURCES:.s=.o)

all: kernel.bin

kernel.bin: $(OBJS)
	@echo "LD     $@"
	@$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	@echo "CC     $<"
	@$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	@echo "AS     $<"
	@$(AS) $(AFLAGS) -o $@ $<

run: os
	@echo "Running in QEMU..."
	qemu-system-i386 -cdrom image/ArcOS-Salvador1.4k.iso -m 512M -boot d

os: kernel.bin
	@echo "Building ISO..."
	mkdir -p image/boot/grub
	cp kernel.bin image/boot/
	cp boot/grub.cfg image/boot/grub/grub.cfg
	grub-mkrescue -o image/ArcOS-Salvador1.4k.iso image

clean:
	@echo "Cleaning up..."
	rm -rf *.o kernel.bin image
	rm src/*.o


