# ArcOS

ArcOS is a os made for fun and not meant for serious stuff.it is x86 that means 32 bit system.works on 2 megabyte but the bootloader grub wont load if lower.future modifications for this os is kinda limited.

---

## What's new in 1.4k

 -Login system
 -Fixed bugs

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
