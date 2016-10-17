#include "paging.h"
#include "lib.h"

/* declerations of kernel page_directory and page_table */
pde_t page_directory[PD_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
pte_t page_table[PT_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));


/* local functions */
void init_first_4MB();
void init_kernel_memory();
void init_rest_of_memory();


/*
 * void init_paging()
 * DESCRIPTION: sets up paging and initializes the page descriptor table for the kernel.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes paging and its nedded parts
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

/*
 * void init_first_4MB()
 * DESCRIPTION: Initializes page descriptor entries to not present for 0MB to 4MB.
 *				This function also initializes the page table that the page_directory
 * 				points to and sets up the video memory portion of memory as present.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes page descriptor
*/
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

/*
 * void init_kernel_memory()
 * DESCRIPTION: initializes page descriptor entries to not present for 4MB to 8MB
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes page descriptor
*/
void init_kernel_memory()
{
	page_directory[KERNEL_PDE_ENTRY] = PDE_ADDRESS_ASSIGN(KERNEL_BASE_ADDRESS);
	page_directory[KERNEL_PDE_ENTRY] = page_directory[KERNEL_PDE_ENTRY] | PDE_ENTRY_4MB_PRESENT;
}

/*
 * void init_rest_of_memory()
 * DESCRIPTION: initializes page descriptor entries to not present for 8MB to 4GBs
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes page descriptor
*/
void init_rest_of_memory()
{
	int i;
	for(i = NOT_PRESENT_MEMORY_START; i < PD_ENTRIES; i++)
	{
		page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
		page_directory[i] = page_directory[i] | PDE_ENTRY_NOT_PRESENT;
	}
}
