#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"
#include "rtc.h"
#include "filesystem.h"
#include "keyboard.h"
#include "x86_desc.h"

// constants and other masks
#define MASK_8KB_ALIGNED 0xFFFFE000
#define MAX_FD_PER_PROCESS 8

// Jump table sub-types
typedef asmlinkage int32_t(*open_func)(const uint8_t*);
typedef asmlinkage int32_t(*close_func)(int32_t);
typedef asmlinkage int32_t(*write_func)(int32_t, const void*, int32_t);
typedef asmlinkage int32_t(*read_func)(int32_t, void*, int32_t);

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
	file_operations_t* jmp_table;
	inode_t* file_inode;
	int32_t file_position;
	int32_t flags;
} file_info_t;

// Structure for Process Control Block
typedef struct
{
	file_info_t fd_array[MAX_FD_PER_PROCESS]; // File descriptors allocated for this process
	uint32_t esp0; // Parent's stack pointer
	uint32_t esp; // Stack pointer
	uint32_t ebp; // Base pointer
	unsigned char* args; // Program's arguments
	uint8_t pid; // Process id

	// Maybe store IRET context of the parent in the child's PCB?
	// Or not, because we jump to parent's execute in halt
} pcb_t;

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

const file_operations_t rtc_table = {
	.o_func = open_rtc;
	.c_func = close_rtc;
	.r_func = read_rtc;
	.w_func = write_rtc;
};

const file_operations_t stdout_table = {
	.o_func = open_terminal;
	.c_func = close_terminal;
	.r_func = read_terminal;
	.w_func = write_terminal;
};

const file_operations_t stdin_table = {
	.o_func = open_keyboard;
	.c_func = close_keyboard;
	.r_func = read_keyboard;
	.w_func = write_keyboard;
};

const file_operations_t file_table = {
	.o_func = open_file;
	.c_func = close_file;
	.r_func = read_file;
	.w_func = write_file;
};

const file_operations_t directory_table = {
	.o_func = open_directory;
	.c_func = close_directory;
	.r_func = read_directory;
	.w_func = write_directory;
};

// set_handler and sigreturn: TODO for extra credit

#endif
