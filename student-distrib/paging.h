#ifndef PAGING_H
#define PAGING_H

#include "lib.h"

/* base address of kernel in both physical and virtual memory */
#define KERNEL_BASE_ADDRESS 0x400000
#define TWENTY_BIT_ADDRESS_MASK 0xFFFFF000
#define VIDEO_MEM_BASE 0xB8000

/* Macros that return the aligned address values of the directory and table entries */
#define PDE_ADDRESS(directory_entry) (directory_entry & (TWENTY_BIT_ADDRESS_MASK))
#define PTE_ADDRESS(table_entry) (table_entry & (TWENTY_BIT_ADDRESS_MASK))
#define PDE_ADDRESS_ASSIGN(address) ((unsigned long) address & (TWENTY_BIT_ADDRESS_MASK))
#define PTE_ADDRESS_ASSIGN(address) ((unsigned long) address & (TWENTY_BIT_ADDRESS_MASK))

/* PTE Entry */
#define PTE_ENTRY_PRESENT 0x000003
#define PTE_ENTRY_NOT_PRESENT 0x000002				// defaults value only allow read/write

/* PDE Entry */
#define PDE_ENTRY_4MB_PRESENT 0x00083
#define PDE_ENTRY_4MB_NOT_PRESENT 0x00082			// defaults value only allow read/write
#define PDE_ENTRY_PRESENT 0x000003
#define PDE_ENTRY_NOT_PRESENT 0x000002				// defaults value only allow read/write
#define PDE_USER_ACCESSIBLE 0x4 // setting this bit in a page directory entry enables user-mode access (i.e. ring3)

/* General constants */
#define PD_ENTRIES 1024
#define PT_ENTRIES 1024
#define BYTES_TO_ALIGN_TO 4096
#define BYTES_TO_ALIGN_4MB 0x400000
#define KERNEL_PDE_ENTRY 1
#define PROCESS_BLOCKS_START 2
#define MAX_TASKS 6
#define NOT_PRESENT_MEMORY_START (PROCESS_BLOCKS_START + MAX_TASKS)

// user access to vidmem
#define SHIFT_4KB 12
#define GRANT_USER_ACCESS 0x0000004;


#define TASK_VIRTUAL_BASE_ADDRESS 0x08000000 // (128MB)
#define TASK_PROGRAM_IMAGE_OFFSET 0x00048000

/* typedef to so that we can have some abstraction */
typedef unsigned long pde_t;
typedef unsigned long pte_t;

/* so that init function can be used in kernel.c */
extern void init_paging();

extern pde_t* get_kernel_page_directory();
extern pde_t* get_page_directory_for_pid(int pid);
extern pte_t* get_base_page_table_for_pid(int pid);
extern void setup_task_paging(pde_t* page_directory, pte_t* base_page_table, uint32_t phys_program_address);

/* written in paging_asm.S */
extern void paging_hw_enable(pde_t* base);
extern void set_new_page_directory(pde_t* base);

extern void setup_user_access_to_vidmem(pte_t* base_page_table, uint8_t * videomem);


#endif
