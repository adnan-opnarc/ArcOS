# ArcOS

ArcOS is a os made for fun and not meant for serious stuff.it is x86 that means 32 bit system.works on 2 megabyte but the bootloader grub wont load if lower.future modifications for this os is kinda limited.

---

## What's new in 1.4k

- Login system
- Fixed bugs

---

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
cd ArcOS
make run
```
## Login password and user

```bash
root(user)
arcroot(password)
```
### File structure
```bash
├── boot
│   ├── grub
│   │   └── themes
│   │       └── arc
│   │           └── theme.txt
│   └── grub.cfg
├── LICENSE
├── linker.ld
├── Makefile
├── README.md
└── src
    ├── crp
    │   ├── ata.c
    │   ├── ata.h
    │   ├── io.h
    │   ├── string.c
    │   └── string.h
    ├── driver
    │   ├── keyboard.c
    │   └── keyboard.h
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
    │   └── kernel.c
    ├── sh
    │   ├── cd.c
    │   ├── cd.h
    │   ├── echo.c
    │   ├── echo.h
    │   ├── exit.c
    │   ├── exit.h
    │   ├── lfetch.c
    │   ├── lfetch.h
    │   ├── ls.c
    │   ├── ls.h
    │   ├── ltime.c
    │   ├── ltime.h
    │   ├── mkdir.c
    │   ├── mkdir.h
    │   ├── rmdir.c
    │   ├── rmdir.h
    │   ├── shell.c
    │   └── shell.h
    └── uilib
        ├── ui.c
        └── ui.h
```

