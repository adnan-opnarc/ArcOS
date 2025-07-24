# ArcOS

ArcOS is a custom bare-metal operating system developed from scratch. It targets x86 32-bit architecture and runs with a minimum of 2MB RAM. The OS features a basic shell, essential command-line tools, and a FAT32 filesystem.

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
make
make run
