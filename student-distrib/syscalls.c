#include "syscalls.h"

asmlinkage int32_t halt(uint8_t status)
{
	return 0;	
}

asmlinkage int32_t execute(const uint8_t* command)
{
	return 0;	
}

asmlinkage int32_t read(int32_t fd, void* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t write(int32_t fd, const void* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t open(const uint8_t* filename)
{
	// Grab esp0 from TSS so that we can access the PCB
	tss_t* tss_base = tss;
	uint32_t esp0 = tss.esp0;

	// Mask bottom 13 bits to get the starting address of the PCB
	// Valid as PCB is at top of kernel stack, which is 8KB-aligned
	pcb_t* pcb = esp0 & MAX_8KB_ALIGNED;

	dentry_t entry;

	// Get dentry for this file
	if(read_dentry_by_name(filename, &entry) == FAILURE)
		return FAILURE; // Return -1 if this isn't a valid file

	// Allocate an unused file descriptor if possible
	for(int i = 2; i < MAX_FD_PER_PROCESS; i++) // Start at 2 as STDIN and STDOUT are always open
	{
		
	}

	// Decide whether we're opening RTC, the directory (.), or another file

	return 0;
}

asmlinkage int32_t close(int32_t fd)
{
	return 0;
}

asmlinkage int32_t getargs(uint8_t* buf, int32_t num_bytes)
{
	return 0;
}

asmlinkage int32_t vidmap(uint8_t** screen_start)
{
	return 0;
}
