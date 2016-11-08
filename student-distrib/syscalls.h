#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"
#include "rtc.h"
#include "filesystem.h"
#include "keyboard.h"

// Jump table types
typedef asmlinkage int32_t(*open_func)(const uint8_t*);
typedef asmlinkage int32_t(*close_func)(int32_t);
typedef asmlinkage int32_t(*write_func)(int32_t, const void*, int32_t);
typedef asmlinkage int32_t(*read_func)(int32_t, void*, int32_t);

// System calls for our OS
// Use asmlinkage to ensure arguments are passed via the stack (useful b/c assembly linkage)
extern asmlinkage int32_t halt(uint8_t status);
extern asmlinkage int32_t execute(const uint8_t* command);
extern asmlinkage int32_t read(int32_t fd, void* buf, int32_t num_bytes);
extern asmlinkage int32_t write(int32_t fd, const void* buf, int32_t num_bytes);
extern asmlinkage int32_t open(const uint8_t* filename);
extern asmlinkage int32_t close(int32_t fd);
extern asmlinkage int32_t getargs(uint8_t* buf, int32_t num_bytes);
extern asmlinkage int32_t vidmap(uint8_t** screen_start);

// Jump tables
open_func open_jmp_table[2] = {open_rtc, open_file};
close_func close_jmp_table[2] = {close_rtc, close_file};
write_func write_jmp_table[3] = {write_rtc, write_file, write_terminal};
read_func read_jmp_table[3] = {read_rtc, read_file, read_terminal};

// set_handler and sigreturn: TODO for extra credit

#endif
