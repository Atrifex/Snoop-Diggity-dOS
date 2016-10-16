#ifndef PAGING_H
#define PAGING_H

// base address of kernel in both physical and virtual memory
#define KERNEL_BASE_ADDRESS 0x400000
#define TWENTY_BIT_ADDRESS_MASK 0xFFFFF000

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

#define VIDEO_MEM_BASE 0xB8000

#define PD_ENTRIES 1024
#define PT_ENTRIES 1024
#define BYTES_TO_ALIGN_TO 4096
#define BYTES_TO_ALIGN_4MB 0x400000
#define NOT_PRESENT_MEMORY_START 2
#define KERNEL_PDE_ENTRY 1

typedef unsigned long pde_t;
typedef unsigned long pte_t;

extern pde_t page_directory[PD_ENTRIES];
extern pte_t page_table[PT_ENTRIES];

extern void init_paging();

// written in paging_asm.S
extern void paging_hw_enable(pde_t* base);

#endif
