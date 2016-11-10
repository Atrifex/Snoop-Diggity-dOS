#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include "filesystem.h"
#include "syscalls.h"

extern int get_available_pid();
extern int mark_pid_used(int pid);
extern int mark_pid_free(int pid);
extern uint32_t block_address_for_process(int pid);

#endif
