# ArcOS
It's a dream for nothing.hope for anything

---

## 1.5kvf-rc1

- Kernel panic
- Cryptic language support working

---
## What's in work
- Cryptic language


## System Requirements

- Electricity
---

## Building and Running

```bash
cd ArcOS
chmod +x build.sh
./build.sh
```
## Login password and user

```bash
root(user)
arcroot(password)
```
### File structure (20% of the shell commands are empty)
```bash
├── binlink.ld
├── boot
│   ├── grub
│   │   └── themes
│   │       └── arc
│   │           └── theme.txt
│   ├── grubarch64.cfg
│   └── grub.cfg
├── build.sh
├── image
├── LICENSE
├── Makefile
├── README.md
├── src
│   └── x86
│       ├── cr
│       │   ├── ccrp.c
│       │   └── ccrp.h
│       ├── crp
│       │   ├── ata.c
│       │   ├── ata.h
│       │   ├── io.h
│       │   ├── string.c
│       │   └── string.h
│       ├── driver
│       │   ├── keyboard.c
│       │   └── keyboard.h
│       ├── fs
│       │   ├── btfs.c
│       │   ├── btfs.h
│       │   ├── fat12.c
│       │   ├── fat12.h
│       │   ├── fat32.c
│       │   ├── fat32.h
│       │   ├── lfs.c
│       │   ├── lfs.h
│       │   ├── nfs.c
│       │   └── nfs.h
│       ├── root
│       │   ├── kernel.c
│       │   ├── kernel.h
│       │   ├── kernel_panic.c
│       │   └── kernel_panic.h
│       ├── sh
│       │   ├── cd.c
│       │   ├── cd.h
│       │   ├── echo.c
│       │   ├── echo.h
│       │   ├── exit.c
│       │   ├── exit.h
│       │   ├── lfetch.c
│       │   ├── lfetch.h
│       │   ├── ls.c
│       │   ├── ls.h
│       │   ├── ltime.c
│       │   ├── ltime.h
│       │   ├── mkdir.c
│       │   ├── mkdir.h
│       │   ├── rmdir.c
│       │   ├── rmdir.h
│       │   ├── shell.c
│       │   └── shell.h
│       └── uilib
│           ├── ui.c
│           └── ui.h
└── utils
    └── flasher.py

```

