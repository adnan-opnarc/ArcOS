#include "script.h"
#include "../fs/ramfs.h"
#include "shell.h"
#include "touch.h"
#include "cat.h"
#include "ls.h"
#include "../crp/string.h"
#include <stdint.h>

#define MAX_SCRIPT_LINE 256
#define MAX_SCRIPT_SIZE 4096

// Execute a shell script from file
int script_execute(const char* script_path) {
    uint8_t script_data[MAX_SCRIPT_SIZE];
    uint32_t bytes_read = 0;
    
    // Read script file
    if (ramfs_read(script_path, script_data, sizeof(script_data) - 1, &bytes_read) != 0) {
        printk("Script not found: ");
        printk(script_path);
        putchar('\n');
        return -1;
    }
    
    script_data[bytes_read] = '\0';
    
    // Execute commands line by line
    return script_execute_commands((const char*)script_data);
}

// Execute commands from a string
int script_execute_commands(const char* commands) {
    const char* line_start = commands;
    char line[MAX_SCRIPT_LINE];
    int line_idx = 0;
    int command_count = 0;
    
    while (*line_start) {
        // Skip whitespace
        while (*line_start == ' ' || *line_start == '\t') line_start++;
        
        // Skip empty lines
        if (*line_start == '\n' || *line_start == '\r' || *line_start == '\0') {
            line_start++;
            continue;
        }
        
        // Skip comments
        if (*line_start == '#') {
            // Skip to end of line
            while (*line_start != '\n' && *line_start != '\r' && *line_start != '\0') {
                line_start++;
            }
            continue;
        }
        
        // Read line
        line_idx = 0;
        while (*line_start != '\n' && *line_start != '\r' && *line_start != '\0' && 
               line_idx < MAX_SCRIPT_LINE - 1) {
            line[line_idx++] = *line_start++;
        }
        line[line_idx] = '\0';
        
        // Skip newline
        if (*line_start == '\n' || *line_start == '\r') line_start++;
        
        // Execute command by calling shell command functions
        // (No debug output - just execute silently)
        if (strncmp(line, "echo ", 5) == 0) {
            const char* text = line + 5;
            while (*text == ' ') text++;
            // Check for redirection
            const char* redirect = text;
            while (*redirect && *redirect != '>') redirect++;
            if (*redirect == '>') {
                // File redirection: echo > file text
                const char* filename = redirect + 1;
                while (*filename == ' ') filename++;
                const char* file_text = filename;
                while (*file_text && *file_text != ' ') file_text++;
                if (*file_text == ' ') {
                    file_text++;
                    char fname[64];
                    int i = 0;
                    while (*filename != ' ' && *filename != '\0' && i < 63) {
                        fname[i++] = *filename++;
                    }
                    fname[i] = '\0';
                    ramfs_write(fname, (const uint8_t*)file_text, strlen(file_text));
                }
            } else {
                printk(text);
                putchar('\n');
            }
        } else if (strncmp(line, "touch ", 6) == 0) {
            const char* filename = line + 6;
            while (*filename == ' ') filename++;
            touch_main(filename);
        } else if (strcmp(line, "ls") == 0) {
            ls_main();
        } else if (strncmp(line, "cat ", 4) == 0) {
            const char* filename = line + 4;
            while (*filename == ' ') filename++;
            cat_main(filename);
        } else if (strncmp(line, "mkdir ", 6) == 0) {
            const char* dirname = line + 6;
            while (*dirname == ' ') dirname++;
            // Create directory with the name
            if (ramfs_mkdir(dirname) == 0) {
                printk("Directory created: ");
                printk(dirname);
                putchar('\n');
            } else {
                printk("Failed to create directory\n");
            }
        } else if (strcmp(line, "clear") == 0) {
            clear_screen();
        } else if (line[0] != '\0') {
            printk("Unknown command: ");
            printk(line);
            putchar('\n');
        }
        
        command_count++;
    }
    
    return command_count;
}

