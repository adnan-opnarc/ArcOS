# ArcOS(legacy)
It's a dream for nothing.hope for anything

---

## 1.7kvf-rc1

- TUI

---
## What's in work
- Out of development


## System Requirements

- Electricity
---

## Building and Running

```bash
cd ArcOS
chmod +x build.sh
./build.sh

or Makefile: make ,make run
```
## Login password and user

```bash
root(user)
arcroot(password)
```
### File structure (20% of the shell commands are empty)
```bash
.
├── binlink.ld
├── boot
│   ├── grub
│   │   └── themes
│   │       └── arc
│   │           └── theme.txt
│   └── grub.cfg
├── build
├── build.sh
├── image
├── LICENSE
├── Makefile
├── README.md
├── src
│   └── x86
│       ├── cr
│       │   ├── ccrp.c
│       │   ├── ccrp.h
│       │   ├── input.c
│       │   ├── input.h
│       │   ├── print.c
│       │   └── print.h
│       ├── crp
│       │   ├── ata.c
│       │   ├── ata.h
│       │   ├── ata.o
│       │   ├── bitmap.c
│       │   ├── bitmap.h
│       │   ├── bitmap_usage.md
│       │   ├── bpm.c
│       │   ├── bpm.h
│       │   ├── io.h
│       │   ├── string.c
│       │   └── string.h
│       ├── driver
│       │   ├── display.c
│       │   ├── display.h
│       │   ├── keyboard.c
│       │   └── keyboard.h
│       ├── fonts
│       │   ├── cp850-8x16.psf
│       │   ├── cp.psf
│       │   └── op.psf
│       ├── fs
│       │   ├── btfs.c
│       │   ├── btfs.h
│       │   ├── btfs.o
│       │   ├── fat12.c
│       │   ├── fat12.h
│       │   ├── fat12.o
│       │   ├── fat32.c
│       │   ├── fat32.h
│       │   ├── lfs.c
│       │   ├── lfs.h
│       │   ├── lfs.o
│       │   ├── nfs.c
│       │   ├── nfs.h
│       │   ├── nfs.o
│       │   ├── ramfs.c
│       │   └── ramfs.h
│       ├── icons
│       │   ├── icon.c
│       │   └── icon.h
│       ├── root
│       │   ├── font_array.h
│       │   ├── font.c
│       │   ├── font_example.c
│       │   ├── font.h
│       │   ├── kernel.c
│       │   ├── kernel.h
│       │   ├── tui.c
│       │   └── tui.h
│       ├── sh
│       │   ├── bitmap.c
│       │   ├── bitmap.h
│       │   ├── cat.c
│       │   ├── cat.h
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
│       │   ├── man.c
│       │   ├── man.h
│       │   ├── mkdir.c
│       │   ├── mkdir.h
│       │   ├── pid.c
│       │   ├── pid.h
│       │   ├── rmdir.c
│       │   ├── rmdir.h
│       │   ├── script.c
│       │   ├── script.h
│       │   ├── shell.c
│       │   ├── shell.h
│       │   ├── touch.c
│       │   └── touch.h
│       └── sys32
│           ├── inf.c
│           └── inf.h
└── utils
    ├── flasher.py
    ├── image_to_carray.py
    └── psf_to_carray.py
```

