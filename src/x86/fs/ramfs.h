#ifndef RAMFS_H
#define RAMFS_H

#include <stdint.h>
#include <stddef.h>

#define RAMFS_MAX_FILES 128
#define RAMFS_MAX_NAME 64
#define RAMFS_MAX_PATH 256
#define RAMFS_FILE_SIZE 4096

// File entry structure
typedef struct {
    char name[RAMFS_MAX_NAME];
    uint8_t* data;
    uint32_t size;
    uint32_t capacity;
    uint8_t is_dir;
    uint8_t in_use;
} ramfs_file_t;

// Filesystem functions
int ramfs_init(void);
int ramfs_mkdir(const char* path);
int ramfs_create_file(const char* path);
int ramfs_write(const char* path, const uint8_t* data, uint32_t size);
int ramfs_read(const char* path, uint8_t* buffer, uint32_t max_size, uint32_t* bytes_read);
int ramfs_list(const char* path, char* output, uint32_t output_size);
int ramfs_delete(const char* path);
int ramfs_exists(const char* path);
uint32_t ramfs_get_size(const char* path);

#endif

