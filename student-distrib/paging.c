#include "paging.h"
#include "lib.h"

pde_t page_directory[PD_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
pte_t page_table[PT_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));

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
	init_video_memory();

	// set up second 4MB (kernel-space memory) - page_directory entry 1
	
	// set up 8MB to 4GB - reset of page_directory

	// enable paging
	paging_hw_enable(page_directory);
}


void init_video_memory()
{
	int i;



	for(i=0; i < PT_ENTRIES; i++)
	{
		if(i*BYTES_TO_ALIGN_TO == 0xB8000)
		{
			page_table[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO)
		}	
		else
		{
			page_table[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO)
		}	
	}
}