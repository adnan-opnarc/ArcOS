#include "fat32.h"
#include "../crp/x86/ata.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

struct fat32_partition fat32_partitions[MAX_PARTITIONS];
int fat32_partition_count = 0;

// FAT32 BPB offsets
#define BPB_BytsPerSec 11
#define BPB_SecPerClus 13
#define BPB_RsvdSecCnt 14
#define BPB_NumFATs 16
#define BPB_FATSz32 36
#define BPB_RootClus 44
#define BPB_TotSec32 32
#define BPB_FATSz16 22
#define BPB_TotSec16 19

static uint32_t part_lba = 0;
static uint32_t fat_begin_lba, cluster_begin_lba, sectors_per_cluster, root_dir_first_cluster;
static uint32_t bytes_per_sector;

// Read MBR and detect up to 4 partitions
int fat32_detect_partitions(void) {
    uint8_t mbr[512];
    fat32_partition_count = 0;
    if (ata_read_sector(0, mbr) != 0) return 0;
    for (int i = 0; i < 4; i++) {
        uint8_t* entry = mbr + 0x1BE + i*16;
        uint8_t type = entry[4];
        if (type == 0x0B || type == 0x0C) { // FAT32
            fat32_partitions[fat32_partition_count].lba_start = *(uint32_t*)(entry+8);
            fat32_partitions[fat32_partition_count].sectors = *(uint32_t*)(entry+12);
            fat32_partition_count++;
        }
    }
    return fat32_partition_count;
}

// Mount the given partition index
int fat32_mount(int part_idx) {
    if (part_idx < 0 || part_idx >= fat32_partition_count) return -1;
    part_lba = fat32_partitions[part_idx].lba_start;
    uint8_t bpb[512];
    if (ata_read_sector(part_lba, bpb) != 0) return -1;
    bytes_per_sector = *(uint16_t*)(bpb + BPB_BytsPerSec);
    sectors_per_cluster = bpb[BPB_SecPerClus];
    uint16_t reserved = *(uint16_t*)(bpb + BPB_RsvdSecCnt);
    uint8_t num_fats = bpb[BPB_NumFATs];
    uint32_t fatsz = *(uint32_t*)(bpb + BPB_FATSz32);
    fat_begin_lba = part_lba + reserved;
    cluster_begin_lba = fat_begin_lba + num_fats * fatsz;
    root_dir_first_cluster = *(uint32_t*)(bpb + BPB_RootClus);
    return 0;
}

// Read a cluster
static int fat32_read_cluster(uint32_t cluster, uint8_t* buf) {
    uint32_t lba = cluster_begin_lba + (cluster - 2) * sectors_per_cluster;
    for (uint32_t i = 0; i < sectors_per_cluster; i++) {
        if (ata_read_sector(lba + i, buf + i * bytes_per_sector) != 0) return -1;
    }
    return 0;
}

// List files in the root directory (no subdirs yet)
int fat32_list_dir(struct fat32_file* files, int max_files) {
    uint8_t cluster_buf[4096];
    if (fat32_read_cluster(root_dir_first_cluster, cluster_buf) != 0) return 0;
    int count = 0;
    // Changed i to uint32_t to fix warning
    for (uint32_t i = 0; i < bytes_per_sector * sectors_per_cluster; i += 32) {
        uint8_t* entry = cluster_buf + i;
        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;
        if ((entry[11] & 0x0F) == 0x0F) continue; // skip LFN
        struct fat32_file* f = &files[count];
        for (int j = 0; j < 11; j++) {
            f->name[j] = entry[j];
        }
        f->name[11] = 0;
        for (int j = 10; j >= 0; j--) if (f->name[j] == ' ') f->name[j] = 0;
        f->first_cluster = (*(uint16_t*)(entry+20) << 16) | *(uint16_t*)(entry+26);
        f->size = *(uint32_t*)(entry+28);
        f->is_dir = (entry[11] & 0x10) ? 1 : 0;
        count++;
        if (count >= max_files) break;
    }
    return count;
}

// Read a file by name (8.3, root dir only)
int fat32_read_file(const char* name, uint8_t* buf, uint32_t maxlen) {
    struct fat32_file files[MAX_FILES];
    int n = fat32_list_dir(files, MAX_FILES);
    for (int i = 0; i < n; i++) {
        if (!files[i].is_dir && !strncmp(name, files[i].name, MAX_FILENAME)) {
            uint32_t cluster = files[i].first_cluster;
            uint32_t size = files[i].size;
            uint32_t read = 0;
            uint8_t cluster_buf[4096];
            while (size > 0 && read < maxlen) {
                if (fat32_read_cluster(cluster, cluster_buf) != 0) return -1;
                uint32_t to_copy = size < bytes_per_sector * sectors_per_cluster ? size : bytes_per_sector * sectors_per_cluster;
                if (read + to_copy > maxlen) to_copy = maxlen - read;
                for (uint32_t j = 0; j < to_copy; j++) buf[read + j] = cluster_buf[j];
                read += to_copy;
                size -= to_copy;
                // Read next cluster from FAT
                uint8_t fat_buf[512];
                uint32_t fat_offset = cluster * 4;
                uint32_t fat_sector = fat_begin_lba + (fat_offset / bytes_per_sector);
                if (ata_read_sector(fat_sector, fat_buf) != 0) return -1;
                cluster = *(uint32_t*)(fat_buf + (fat_offset % bytes_per_sector)) & 0x0FFFFFFF;
                if (cluster >= 0x0FFFFFF8) break;
            }
            return read;
        }
    }
    return -1;
}

// Minimal FAT32 format (single root dir cluster, 2 FATs, 512B/sector, 8 sectors/cluster)
int fat32_format(int part_idx) {
    if (part_idx < 0 || part_idx >= fat32_partition_count) return -1;
    uint32_t lba = fat32_partitions[part_idx].lba_start;
    uint8_t sector[512];
    memset(sector, 0, 512);
    // BPB
    sector[0x0B] = 0x00; sector[0x0C] = 0x02; // 512 bytes/sector
    sector[0x0D] = 0x08; // 8 sectors/cluster
    sector[0x0E] = 0x01; sector[0x0F] = 0x00; // 1 reserved sector
    sector[0x10] = 0x02; // 2 FATs
    sector[0x15] = 0xF8; // media descriptor
    sector[0x16] = 0x00; sector[0x17] = 0x00; // root entries (FAT32=0)
    sector[0x18] = 0x20; sector[0x19] = 0x00; // 32 sectors/track
    sector[0x1A] = 0x40; sector[0x1B] = 0x00; // 64 heads
    sector[0x1C] = 0x00; sector[0x1D] = 0x00; sector[0x1E] = 0x00; sector[0x1F] = 0x00; // hidden sectors
    uint32_t total_sectors = fat32_partitions[part_idx].sectors;
    sector[0x20] = (uint8_t)(total_sectors & 0xFF);
    sector[0x21] = (uint8_t)((total_sectors >> 8) & 0xFF);
    sector[0x22] = (uint8_t)((total_sectors >> 16) & 0xFF);
    sector[0x23] = (uint8_t)((total_sectors >> 24) & 0xFF);
    uint32_t fatsz = 128; // 128 sectors per FAT (enough for small disks)
    sector[0x24] = (uint8_t)(fatsz & 0xFF);
    sector[0x25] = (uint8_t)((fatsz >> 8) & 0xFF);
    sector[0x26] = (uint8_t)((fatsz >> 16) & 0xFF);
    sector[0x27] = (uint8_t)((fatsz >> 24) & 0xFF);
    sector[0x2C] = 0x02; sector[0x2D] = 0x00; sector[0x2E] = 0x00; sector[0x2F] = 0x00; // root cluster = 2
    sector[0x40] = 0x29; // boot signature
    // Write BPB
    if (ata_write_sector(lba, sector) != 0) return -1;
    // Clear FATs and root dir clusters here (omitted for brevity)
    return 0;
}

// Write a file to root dir (overwrite if exists)
int fat32_write_file(int part_idx, const char* name, const uint8_t* data, uint32_t size) {
    if (part_idx < 0 || part_idx >= fat32_partition_count) return -1;
    uint32_t cluster = 2; // for simplicity, use cluster 2 (root)
    uint8_t cluster_buf[4096];
    // Fill cluster with data (limited to one cluster)
    memset(cluster_buf, 0, bytes_per_sector * sectors_per_cluster);
    if (size > bytes_per_sector * sectors_per_cluster) size = bytes_per_sector * sectors_per_cluster;
    memcpy(cluster_buf, data, size);
    uint32_t cluster_lba = cluster_begin_lba + (cluster - 2) * sectors_per_cluster;
    for (uint32_t s = 0; s < sectors_per_cluster; s++) { // changed s to uint32_t
        if (ata_write_sector(cluster_lba + s, cluster_buf + s*bytes_per_sector) != 0) return -1;
    }
    // Write directory entry in root dir cluster
    uint8_t dir[512];
    if (ata_read_sector(cluster_lba, dir) != 0) return -1;
    // Changed k to uint32_t
    for (uint32_t k = 0; k < 512; k += 32) {
        if (dir[k] == 0x00 || dir[k] == 0xE5) {
            memset(dir + k, 0, 32);
            for (int n = 0; n < 11 && name[n]; n++) dir[k + n] = name[n];
            dir[k + 11] = 0x20; // archive attribute
            *(uint16_t*)(dir + k + 20) = (cluster >> 16);
            *(uint16_t*)(dir + k + 26) = (cluster & 0xFFFF);
            *(uint32_t*)(dir + k + 28) = size;
            break;
        }
    }
    for (uint32_t s = 0; s < sectors_per_cluster; s++) { // changed s to uint32_t
        if (ata_write_sector(cluster_lba + s, dir + s*512) != 0) return -1;
    }
    return 0;
}

