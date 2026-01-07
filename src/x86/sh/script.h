#ifndef SCRIPT_H
#define SCRIPT_H

// Execute a shell script from file
int script_execute(const char* script_path);

// Execute commands from a string (for scripting)
int script_execute_commands(const char* commands);

#endif

