#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "lib.h"
#include "types.h"
#include "rtc.h"
#include "filesystem.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "paging.h"
#include "scheduling.h"

// constants and other masks
#define MASK_8KB_ALIGNED 0xFFFFE000
#define ISOLATE_BIT_0 0x01
#define MIN_FD_PER_PROCESS 2
#define MAX_FD_PER_PROCESS 8
#define MAX_EXECUTE_ARG_SIZE 128
#define KERNEL_PID -1
#define USER_EXECUTE 0
#define FIRST_TERM_SHELL 1

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
    uint32_t ret_val;
    unsigned char* args; // Program's arguments
	// parent's info
	pcb_t * parentPCB;
	uint32_t esp0; 		// Parent's kernel stack pointer
	uint32_t esp_k; 	// current's kernel stack pointer
	uint32_t ebp_k; 	// current's kernel base pointer

	uint32_t esp_u; 	// current's user stack pointer
	uint32_t ebp_u; 	// current's user base pointer

   /*
    * If set then the corresponding conditions are being met:
    * |-----------+-------------------+---------------|
    * | bits 31-2 | bit 1             | bit 0         |
    * |-----------+-------------------+---------------|
    * | Reserved  | in hard interrupt | initial shell |
    * |-----------+-------------------+---------------|
    */
	uint32_t flags; 	// we'll use this for something

    // kept down here to maximize packing
    int8_t pid;    	// Process id of current process

    // terminal from which this process started
    uint8_t owned_by_terminal;
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
extern asmlinkage int32_t set_handler(int32_t signum, void* handler_address);
extern asmlinkage int32_t sigreturn();
extern int32_t internal_execute(const uint8_t* command, uint32_t flags);
extern void go_to_process(int8_t pid);
extern void save_process_information(uint8_t pid);

// set_handler and sigreturn: TODO for extra credit

// Executable check
#define NUM_MAGIC_CHECK 4
#define EXECUTABLE_CHECK_0 0x7f
#define EXECUTABLE_CHECK_1 0x45
#define EXECUTABLE_CHECK_2 0x4c
#define EXECUTABLE_CHECK_3 0x46

// masks for setting up context swtich
#define SET_IOPRIV_USER 0x00003000
#define SET_INTERRUPTS 0x0000200
#define SET_PF_RANDBIT 0x0006
#define KERNEL_STACK_START (0x800000) //- LITERAL_8KB)
#define ACCOUNT_FOR_RET_ADDR 4
#define ENTRY_POINT_INDEX 24


#define ELF_ID_BYTE_0 0
#define ELF_ID_BYTE_1 1
#define ELF_ID_BYTE_2 2
#define ELF_ID_BYTE_3 3

extern void iret_to_user(unsigned long entry_point_address, unsigned long cs, unsigned long new_flags, unsigned long new_esp, unsigned long ss);
extern uint32_t get_esp();
extern uint32_t get_ebp();

extern int32_t halt_excep(int32_t status);


/*
# set_esp_ebp()
# DESCRIPTION: sets the esp and ebp to a given value
# INPUTS   : uint32_t par_esp, uint32_t par_ebp
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: esp and ebp
*/
#define set_esp_ebp(par_esp, par_ebp) 													            \
    do {                                                                                            \
        asm volatile("\n                                                                            \
                        movl %0, %%esp      \n                                                      \
                        movl %1, %%ebp      \n                                                      \
                     "     	                                                                        \
                     :                                                                              \
                     : "r"(par_esp), "r"(par_ebp)				                                    \
                     : "esp", "ebp", "eax"                                                          \
            );                                                                                      \
    } while(0)

#endif
