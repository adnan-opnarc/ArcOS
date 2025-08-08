#ifndef FAT32_H
#define FAT32_H
#include <stdint.h>

#define MAX_PARTITIONS 4
#define MAX_FILENAME 12
#define MAX_FILES 64

struct fat32_partition {
    uint32_t lba_start;
    uint32_t sectors;
};

struct fat32_file {
    char name[MAX_FILENAME];
    uint32_t first_cluster;
    uint32_t size;
    uint8_t is_dir;
};

extern struct fat32_partition fat32_partitions[MAX_PARTITIONS];
extern int fat32_partition_count;

int fat32_detect_partitions(void);
int fat32_mount(int part_idx);
int fat32_list_dir(struct fat32_file* files, int max_files);
int fat32_read_file(const char* name, uint8_t* buf, uint32_t maxlen);
int fat32_format(int part_idx);
int fat32_write_file(int part_idx, const char* name, const uint8_t* data, uint32_t size);

#endif 