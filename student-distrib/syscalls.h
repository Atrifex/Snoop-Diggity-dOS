#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"
#include "rtc.h"
#include "filesystem.h"
#include "keyboard.h"

// System calls for our OS
// Use fastcall attribute to pass arguments via registers (useful b/c assembly linkage)
extern asmlinkage int32_t halt(uint8_t status);
extern asmlinkage int32_t execute(const uint8_t* command);
extern asmlinkage int32_t read(int32_t fd, void* buf, int32_t num_bytes);
extern asmlinkage int32_t write(int32_t fd, const void* buf, int32_t num_bytes);
extern asmlinkage int32_t open(const uint8_t* filename);
extern asmlinkage int32_t close(int32_t fd);
extern asmlinkage int32_t getargs(uint8_t* buf, int32_t num_bytes);
extern asmlinkage int32_t vidmap(uint8_t** screen_start);

// set_handler and sigreturn: TODO for extra credit

#endif