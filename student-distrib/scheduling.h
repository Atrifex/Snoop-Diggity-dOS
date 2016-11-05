#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include "filesystem.h"

#define MAX_FD_PER_PROCESS 8

// Structure for Process Control Block
typedef struct
{
	int32_t fd_array[MAX_FD_PER_PROCESS]; // File descriptors allocated for this process
	uint32_t esp; // Stack pointer
	uint32_t ebp; // Base pointer
	uint8_t pid; // Process id
} pcb_t;

#endif
