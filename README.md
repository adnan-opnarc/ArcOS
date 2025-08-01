# ArcOS

ArcOS is a os made for fun and not meant for serious stuff.it is x86 that means 32 bit system.works on 2 megabyte but the bootloader grub wont load if lower.future modifications for this os is kinda limited.

---

## What's new in 1.4k

- Login system
- Fixed bugs

---
## What's in work
- Arm cpu support

## Features

- Grub bootloader support
- Minimal command-line shell with:
  - `ls` – List directory contents
  - `clear` – Clear the screen
  - `lfetch` – Display system information
  - `reboot` – Reboot the system
  - `shutdown` – Power off the machine
- FAT32 filesystem access
- PS/2 keyboard input
- Kernel version 1.3

---

## System Requirements

- 32-bit x86 CPU
- At least 2MB RAM
- QEMU or compatible hardware for testing

---

## Building and Running

```bash
cd ArcOS
./build.sh
```
## Login password and user

```bash
root(user)
arcroot(password)
```
### File structure
```bash
.
├── boot
│   ├── grub
│   │   └── themes
│   │       └── arc
│   │           └── theme.txt
│   ├── grubarch64.cfg
│   └── grub.cfg
├── build.sh
├── LICENSE
├── linker.ld
├── Makefile
├── README.md
└── src
    ├── arm
    │   └── ark
    │       ├── ark.elf
    │       ├── include
    │       │   └── uart.h
    │       ├── kernel8.img
    │       ├── Makefile
    │       ├── scripts
    │       │   └── linker.ld
    │       └── src
    │           ├── arch
    │           │   └── arm
    │           │       └── boot
    │           │           └── entry.s
    │           ├── drivers
    │           │   └── uart.c
    │           └── kernel
    │               └── main.c
    └── x86
        ├── crp
        │   └── x86
        │       ├── ata.c
        │       ├── ata.h
        │       ├── io.h
        │       ├── string.c
        │       └── string.h
        ├── driver
        │   └── x86
        │       ├── keyboard.c
        │       └── keyboard.h
        ├── fs
        │   ├── btfs.c
        │   ├── btfs.h
        │   ├── fat12.c
        │   ├── fat12.h
        │   ├── fat32.c
        │   ├── fat32.h
        │   ├── lfs.c
        │   ├── lfs.h
        │   ├── nfs.c
        │   └── nfs.h
        ├── root
        │   └── x86
        │       └── kernel.c
        ├── sh
        │   └── x86
        │       ├── cd.c
        │       ├── cd.h
        │       ├── echo.c
        │       ├── echo.h
        │       ├── exit.c
        │       ├── exit.h
        │       ├── lfetch.c
        │       ├── lfetch.h
        │       ├── ls.c
        │       ├── ls.h
        │       ├── ltime.c
        │       ├── ltime.h
        │       ├── mkdir.c
        │       ├── mkdir.h
        │       ├── rmdir.c
        │       ├── rmdir.h
        │       ├── shell.c
        │       └── shell.h
        └── uilib
            └── x86
                ├── ui.c
                └── ui.h

28 directories, 54 files

```

