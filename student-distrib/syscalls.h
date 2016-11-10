#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "lib.h"
#include "types.h"
#include "rtc.h"
#include "filesystem.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "paging.h"

// constants and other masks
#define MASK_8KB_ALIGNED 0xFFFFE000
#define ISOLATE_BIT_0 0x01
#define MAX_FD_PER_PROCESS 8
#define MAX_EXECUTE_ARG_SIZE 256
#define FOUR_MEGS 0x400000

// Jump table sub-types
typedef int32_t (*open_func)(const uint8_t*);
typedef int32_t (*close_func)(int32_t);
typedef int32_t (*write_func)(int32_t, const void*, int32_t);
typedef int32_t (*read_func)(int32_t, void*, int32_t);

// Jump table type
typedef struct
{
	open_func o_func;
	close_func c_func;
	read_func r_func;
	write_func w_func;
} file_operations_t;

/*
 * Structure storing information about an open file
 *
 * Flag usage:
 *  |-----------+--------|
 *  | bits 31-1 | bit 0  |
 *  |-----------+--------|
 *  | Reserved  | in-use |
 *  |-----------+--------|
 *
 *
 */
typedef struct
{
	file_operations_t* fops_jmp_table;
	inode_t* inode;
	int32_t position;
	int32_t flags;
} file_info_t;

// Structure for Process Control Block
typedef struct pcb_t pcb_t;
struct pcb_t
{
	file_info_t fd_array[MAX_FD_PER_PROCESS]; // File descriptors allocated for this process

	unsigned char* args; // Program's arguments
	uint8_t pid; // Process id of current process

	// parent's info
	uint32_t esp0; // Parent's kernel stack pointer
	uint32_t parent_pid; // process id of parent
	uint32_t esp; // Parent's user stack pointer
	uint32_t ebp; // Parent's user base pointer

	uint32_t flags; // we'll use this for something

	// Maybe store IRET context of the parent in the child's PCB?
	// Or not, because we jump to parent's execute in halt
};

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



// set_handler and sigreturn: TODO for extra credit

#endif
