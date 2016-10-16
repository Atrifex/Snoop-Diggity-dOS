#ifndef PAGING_H
#define PAGING_H

// base address of kernel in both physical and virtual memory
#define KERNEL_BASE_ADDRESS 0x400000

#define PDE_ADDRESS(directory_entry) (directory_entry & (0xFFFFF000))
#define PTE_ADDRESS(table_entry) (table_entry & (0xFFFFF000))

#define PDE_ADDRESS_ASSIGN(address) (address & (0xFFFFF000))
#define PTE_ADDRESS_ASSIGN(address) (address & (0xFFFFF000))

#define PD_ENTRIES 1024
#define PT_ENTRIES 1024
#define BYTES_TO_ALIGN_TO 4096

typedef unsigned long pde_t;
typedef unsigned long pte_t;

extern pde_t page_directory[PD_ENTRIES];
extern pte_t page_table[PT_ENTRIES];

extern void init_paging();

// written in paging_asm.S
extern void paging_hw_enable(pde_t* base);

#endif
