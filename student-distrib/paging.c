#include "paging.h"
#include "lib.h"

pde_t page_directory[PD_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
pte_t page_table[PT_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));


void init_first_4MB();
void init_kernel_memory();
void init_rest_of_memory();


/*
 * init_paging
 * DESCRIPTION: .
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: allows paging
*/
void init_paging()
{
	// set up first 4MB (video memory) - page_directory entry 0
	init_first_4MB();

	// set up second 4MB (kernel-space memory) - page_directory entry 1
	init_kernel_memory();

	// set up 8MB to 4GB - reset of page_directory
	init_rest_of_memory();

	// enable paging
	paging_hw_enable(page_directory);
}


void init_first_4MB()
{
	int i;

	page_directory[0] = PDE_ADDRESS_ASSIGN(page_table);
	page_directory[0] = page_directory[0] | PDE_ENTRY_PRESENT;

	for(i=0; i < PT_ENTRIES; i++)
	{
		if(i*BYTES_TO_ALIGN_TO == VIDEO_MEM_BASE)
		{
			page_table[i] = PTE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO);
			page_table[i] = page_table[i] | PTE_ENTRY_PRESENT;
		}	
		else
		{
			page_table[i] = PTE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO);
			page_table[i] = page_table[i] | PTE_ENTRY_NOT_PRESENT;
		}	
	}
}

void init_kernel_memory()
{
	page_directory[KERNEL_PDE_ENTRY] = PDE_ADDRESS_ASSIGN(KERNEL_BASE_ADDRESS);
	page_directory[KERNEL_PDE_ENTRY] = page_directory[KERNEL_PDE_ENTRY] | PDE_ENTRY_4MB_PRESENT;	
}

void init_rest_of_memory()
{
	int i;
	for(i = NOT_PRESENT_MEMORY_START; i < PD_ENTRIES; i++)
	{
		page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
		page_directory[i] = page_directory[i] | PDE_ENTRY_NOT_PRESENT;
	}
}
