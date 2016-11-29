#include "paging.h"

/* declarations of kernel kernel_page_directory and kernel_page_table */
pde_t kernel_page_directory[PD_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
pte_t kernel_page_table[PT_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));

/* per-program page directories - for now, one shell, one user level program - so only use entries 0 and 1 - but more in future*/
pde_t user_level_page_directories[MAX_TASKS][PD_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));
/* per-program base page tables i.e. these map 0MB-4MB, marking everything as not present but video memory, and setting video memory to supervisor only */
pte_t user_level_base_page_tables[MAX_TASKS][PT_ENTRIES] __attribute__((aligned(BYTES_TO_ALIGN_TO)));


/* local functions */
void init_kernel_pages();
void setup_base_page_table(pte_t* base_page_table);


/*
 * void init_paging()
 * DESCRIPTION: sets up paging and initializes the page directory and singular page table for the kernel.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: initializes paging and its nedded parts
*/
void init_paging()
{
	// set up kernel page directory/table
	init_kernel_pages();

	// enable paging
	paging_hw_enable(kernel_page_directory);
}

/*
 * void init_kernel_pages()
 * DESCRIPTION: Initializes the kernel's page directory and single page table (which maps 0MB-4MB)
 * INPUTS: none
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: initializes kerneL_page_directory and kernel_page_table
*/
void init_kernel_pages()
{
	int i;

	// maps 0MB-4MB to the page table we set up below
	kernel_page_directory[0] = PDE_ADDRESS_ASSIGN(kernel_page_table);
	kernel_page_directory[0] |= PDE_ENTRY_PRESENT;

	// set up the page table which describes 0MB-4MB
	setup_base_page_table(kernel_page_table);

	// map 4MB-8MB to the kernel as a single 4MB page
	kernel_page_directory[KERNEL_PDE_ENTRY] = PDE_ADDRESS_ASSIGN(KERNEL_BASE_ADDRESS);
	kernel_page_directory[KERNEL_PDE_ENTRY] |= PDE_ENTRY_4MB_PRESENT;

	// map 8MB - (8MB + (MAX_TASKS * 4MB)) as present for process blocks
	for(i = PROCESS_BLOCKS_START; i < NOT_PRESENT_MEMORY_START; i++) {
		kernel_page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
		kernel_page_directory[i] |= PDE_ENTRY_4MB_PRESENT;
	}

	// map everything else up to 4GB as not present
	for(i = NOT_PRESENT_MEMORY_START; i < PD_ENTRIES; i++) {
		kernel_page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
		kernel_page_directory[i] |= PDE_ENTRY_NOT_PRESENT;
	}
}

/*
 * pde_t* get_kernel_page_directory()
 * DESCRIPTION: returns a pointer to the kernel page directory
 * INPUTS: none
 * OUTPUTS: none
 * RETURN VALUE: pointer to the page directory
 * SIDE EFFECTS: none
*/
pde_t* get_kernel_page_directory()
{
	return kernel_page_directory;
}



/*
 * pde_t* get_page_directory_for_pid(int pid)
 * DESCRIPTION: Returns the corresponding page directory for a PID or NULL if invalid PID is passed
 * INPUTS: int pid - processs ID to get dir for
 * OUTPUTS: none
 * RETURN VALUE: pointer to the page directory
 * SIDE EFFECTS: none
*/
pde_t* get_page_directory_for_pid(int pid) {
	if(pid >= MAX_TASKS) return NULL;
	return user_level_page_directories[pid];
}

/*
 * pte_t* get_base_page_table_for_pid(int pid)
 * DESCRIPTION: Returns the corresponding base page table for a PID or NULL if invalid PID is passed
 * INPUTS: int pid - processs ID to get dir for
 * OUTPUTS: none
 * RETURN VALUE: pointer to the page table
 * SIDE EFFECTS: none
*/
pte_t* get_base_page_table_for_pid(int pid) {
	if(pid >= MAX_TASKS) return NULL;
	return user_level_base_page_tables[pid];
}

/*
 * void setup_task_paging(pde_t* page_directory, pte_t* base_page_table, uint32_t phys_program_address)
 * DESCRIPTION: Sets up a page directory for a new user-space process.
 * INPUTS: page_directory: pointer to page directory for this process to initialize, base_page_table: pointer to base (0MB-4MB) page table for this process, phys_program_address: physical memory address to map program image to
 * OUTPUTS: page_directory, base_page_table
 * RETURN VALUE: none
 * SIDE EFFECTS: updates passed params
*/
void setup_task_paging(pde_t* page_directory, pte_t* base_page_table, uint32_t phys_program_address)
{
	int i;

	// we are not setting the user bit for this entry, so it will be inaccessible except for the kernel. tihs is what we want.
	// as accessing video memory at VIDEO_MEM_BASE should only be done by the kernel, vidmap will enable userspace level video memory accesss
	page_directory[0] = PDE_ADDRESS_ASSIGN(base_page_table);
	page_directory[0] |= PDE_ENTRY_PRESENT;

	// setup the base page table mapping 0MB-4MB as not present except for video memory
	setup_base_page_table(base_page_table);

	// map 4MB-8MB to the kernel as a single 4MB page
	page_directory[KERNEL_PDE_ENTRY] = PDE_ADDRESS_ASSIGN(KERNEL_BASE_ADDRESS);
	page_directory[KERNEL_PDE_ENTRY] |= PDE_ENTRY_4MB_PRESENT;

	// map 8MB - (8MB + (MAX_TASKS * 4MB)) as present for process blocks
	// these should be only kernel accessible, but uesd from when we call execute
	// within a running task, and need to write to a page for the next proc
	for(i = PROCESS_BLOCKS_START; i < NOT_PRESENT_MEMORY_START; i++) {
		page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
		page_directory[i] |= PDE_ENTRY_4MB_PRESENT;
	}

	// map the program image at 0x08000000 and mark everything else as not present
	for(i = NOT_PRESENT_MEMORY_START; i < PD_ENTRIES; i++) {
		if(i*BYTES_TO_ALIGN_4MB == TASK_VIRTUAL_BASE_ADDRESS) {
			page_directory[i] = PDE_ADDRESS_ASSIGN(phys_program_address);
			page_directory[i] |= PDE_ENTRY_4MB_PRESENT | PDE_USER_ACCESSIBLE;
		} else {
			page_directory[i] = PDE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_4MB);
			page_directory[i] |= PDE_ENTRY_NOT_PRESENT;
		}
	}
}


/*
 * void setup_base_page_table(pte_t* base_page_table)
 * DESCRIPTION: Sets up a "base" (0MB-4MB) page table, pointed to by base_page_table, marking all entries as not present with the exception of video memory
 * INPUTS: base_page_table - pointer to base page table to set up
 * OUTPUTS: base_page_table - updates this table to be configured properly
 * RETURN VALUE: none
 * SIDE EFFECTS: updates base_page_table
*/
void setup_base_page_table(pte_t* base_page_table)
{
	int i;

	// loop all entries of the page table. set the video memory entry to map to physical video memory, mark everything else as not present.
	for(i=0; i < PT_ENTRIES; i++) {
		if(i*BYTES_TO_ALIGN_TO == VIDEO_MEM_BASE
				|| i*BYTES_TO_ALIGN_TO == VIDEO_MEM_BASE + LITERAL_4KB
				|| i*BYTES_TO_ALIGN_TO == VIDEO_MEM_BASE + LITERAL_8KB
				|| i*BYTES_TO_ALIGN_TO == VIDEO_MEM_BASE + LITERAL_12KB)
	 	{
			base_page_table[i] = PTE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO);
			base_page_table[i] |= PTE_ENTRY_PRESENT;
		} else {
			base_page_table[i] = PTE_ADDRESS_ASSIGN(i*BYTES_TO_ALIGN_TO);
			base_page_table[i] |= PTE_ENTRY_NOT_PRESENT;
		}
	}
}

/*
 * void change_table_mapping(pte_t* table, uint32_t address_of_entry, uint32_t new_value)
 * DESCRIPTION: Updates the entry in table corresponding to address_of_entry, setting it to new_value
 * INPUTS: pte_t* table - table to execute, uint32_t address_of_entry - the address of the entry to update, provided it is in table, uint32_t the new value
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: updates the given table
*/
void change_table_mapping(pte_t* table, uint32_t address_of_entry, uint32_t new_value)
{
	// determine index into the page table
	int index = (int) ( (uint32_t)address_of_entry >> SHIFT_4KB );
	index &= ISOLATE_PTE_IDX;

	// grab current entry
	unsigned long entry = table[index];

	// set new mapping
	table[index] = PTE_ADDRESS_ASSIGN((uint32_t) new_value);
	table[index] |= (entry&ISOLATE_BTM_12BITS);
}


/*
 * void setup_user_access_to_vidmem(pte_t* base_page_table, uint8_t * videomem)
 * DESCRIPTION: Sets memory address pointed to by videomem as user accessible in the given page table
 * INPUTS: base_page_table - pointer to page table to configure, videomem address of video mem to update
 * OUTPUTS: base_page_table - updates this table to be configured properly
 * RETURN VALUE: none
 * SIDE EFFECTS: updates base_page_table
*/
void setup_user_access_pte(pte_t* base_page_table, uint8_t * videomem)
{
	int index = (int) ( (uint32_t)videomem >> SHIFT_4KB );
	index &= ISOLATE_PTE_IDX;
	base_page_table[index] |= GRANT_USER_ACCESS;
}

/*
 * void setup_user_access_pde(pde_t* page_directory, uint8_t * videomem)
 * DESCRIPTION: Sets directory of memory address pointed to by videomem as user accessible in the given page directory
 * INPUTS: page_directory - pointer to page directory to configure, videomem address of video mem to update
 * OUTPUTS: page_directory - updates this directory to be configured properly
 * RETURN VALUE: none
 * SIDE EFFECTS: updates page_directory
*/
void setup_user_access_pde(pde_t* page_directory, uint8_t * videomem)
{
	int index = (int) ( (uint32_t)videomem >> SHIFT_4MB );
	page_directory[index] |= GRANT_USER_ACCESS;
}
