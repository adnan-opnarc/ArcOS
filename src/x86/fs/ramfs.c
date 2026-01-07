#include "ramfs.h"
#include "../crp/string.h"
#include <stdint.h>
#include <stddef.h>

static ramfs_file_t files[RAMFS_MAX_FILES];
static uint8_t file_data_pool[RAMFS_MAX_FILES * RAMFS_FILE_SIZE];
static int next_data_offset = 0;

// Initialize filesystem
int ramfs_init(void) {
    // Clear all files
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        files[i].in_use = 0;
        files[i].name[0] = '\0';
        files[i].data = NULL;
        files[i].size = 0;
        files[i].capacity = 0;
        files[i].is_dir = 0;
    }
    next_data_offset = 0;
    
    // Create root directory
    ramfs_mkdir("/");
    return 0;
}

// Find a free file slot
static int find_free_slot(void) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (!files[i].in_use) {
            return i;
        }
    }
    return -1;
}

// Find file by path
static int find_file(const char* path) {
    for (int i = 0; i < RAMFS_MAX_FILES; i++) {
        if (files[i].in_use && strcmp(files[i].name, path) == 0) {
            return i;
        }
    }
    return -1;
}

// Normalize path (remove trailing /, handle //)
static void normalize_path(char* dest, const char* src) {
    int i = 0, j = 0;
    int len = 0;
    while (src[len]) len++;
    
    // Copy and normalize
    while (i < len && j < RAMFS_MAX_NAME - 1) {
        if (src[i] == '/' && (i == 0 || dest[j-1] == '/')) {
            i++; // Skip duplicate /
            continue;
        }
        dest[j++] = src[i++];
    }
    
    // Remove trailing /
    if (j > 1 && dest[j-1] == '/') {
        j--;
    }
    
    dest[j] = '\0';
}

// Create directory
int ramfs_mkdir(const char* path) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    // Check if already exists
    if (find_file(normalized) >= 0) {
        return -1; // Already exists
    }
    
    int slot = find_free_slot();
    if (slot < 0) return -1; // No free slots
    
    files[slot].in_use = 1;
    strcpy(files[slot].name, normalized);
    files[slot].data = NULL;
    files[slot].size = 0;
    files[slot].capacity = 0;
    files[slot].is_dir = 1;
    
    return 0;
}

// Create file
int ramfs_create_file(const char* path) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    // Check if already exists
    int existing = find_file(normalized);
    if (existing >= 0) {
        // File exists, clear it
        files[existing].size = 0;
        return 0;
    }
    
    int slot = find_free_slot();
    if (slot < 0) return -1; // No free slots
    
    if (next_data_offset + RAMFS_FILE_SIZE > sizeof(file_data_pool)) {
        return -1; // Out of memory
    }
    
    files[slot].in_use = 1;
    strcpy(files[slot].name, normalized);
    files[slot].data = file_data_pool + next_data_offset;
    files[slot].size = 0;
    files[slot].capacity = RAMFS_FILE_SIZE;
    files[slot].is_dir = 0;
    
    next_data_offset += RAMFS_FILE_SIZE;
    
    return 0;
}

// Write to file
int ramfs_write(const char* path, const uint8_t* data, uint32_t size) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    int idx = find_file(normalized);
    if (idx < 0) {
        // Create file if it doesn't exist
        if (ramfs_create_file(normalized) < 0) return -1;
        idx = find_file(normalized);
        if (idx < 0) return -1;
    }
    
    if (files[idx].is_dir) return -1; // Can't write to directory
    
    // Limit to capacity
    if (size > files[idx].capacity) {
        size = files[idx].capacity;
    }
    
    // Copy data
    for (uint32_t i = 0; i < size; i++) {
        files[idx].data[i] = data[i];
    }
    
    files[idx].size = size;
    return 0;
}

// Read from file
int ramfs_read(const char* path, uint8_t* buffer, uint32_t max_size, uint32_t* bytes_read) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    int idx = find_file(normalized);
    if (idx < 0) return -1;
    
    if (files[idx].is_dir) return -1; // Can't read directory
    
    uint32_t to_read = files[idx].size;
    if (to_read > max_size) {
        to_read = max_size;
    }
    
    for (uint32_t i = 0; i < to_read; i++) {
        buffer[i] = files[idx].data[i];
    }
    
    if (bytes_read) {
        *bytes_read = to_read;
    }
    
    return 0;
}

// List directory contents
int ramfs_list(const char* path, char* output, uint32_t output_size) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    // If path is "/", list root
    int count = 0;
    output[0] = '\0';
    
    for (int i = 0; i < RAMFS_MAX_FILES && count < output_size - 100; i++) {
        if (!files[i].in_use) continue;
        
        // Check if file is in this directory
        // Simple check: if path is "/", show all root files
        // Otherwise check if file starts with path/
        int match = 0;
        if (strcmp(normalized, "/") == 0) {
            // Root directory - show files without /
            if (strchr(files[i].name, '/') == NULL || 
                (strchr(files[i].name, '/') == files[i].name && 
                 strchr(files[i].name + 1, '/') == NULL)) {
                match = 1;
            }
        } else {
            // Check if file is in subdirectory
            char prefix[RAMFS_MAX_NAME + 2];
            strcpy(prefix, normalized);
            strcat(prefix, "/");
            if (strncmp(files[i].name, prefix, strlen(prefix)) == 0) {
                match = 1;
            }
        }
        
        if (match) {
            const char* display_name = files[i].name;
            if (strcmp(normalized, "/") != 0) {
                display_name = files[i].name + strlen(normalized) + 1;
            }
            
            // Append to output
            int len = strlen(output);
            if (files[i].is_dir) {
                strcpy(output + len, display_name);
                strcat(output + len, "/ ");
            } else {
                strcpy(output + len, display_name);
                strcat(output + len, " ");
            }
            count++;
        }
    }
    
    return count;
}

// Delete file or directory
int ramfs_delete(const char* path) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    int idx = find_file(normalized);
    if (idx < 0) return -1;
    
    // Can't delete root
    if (strcmp(normalized, "/") == 0) return -1;
    
    files[idx].in_use = 0;
    files[idx].name[0] = '\0';
    files[idx].data = NULL;
    files[idx].size = 0;
    
    return 0;
}

// Check if file exists
int ramfs_exists(const char* path) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    return find_file(normalized) >= 0 ? 1 : 0;
}

// Get file size
uint32_t ramfs_get_size(const char* path) {
    char normalized[RAMFS_MAX_NAME];
    normalize_path(normalized, path);
    
    int idx = find_file(normalized);
    if (idx < 0) return 0;
    
    return files[idx].size;
}

