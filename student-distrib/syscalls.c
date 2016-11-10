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

/*
 * int32_t open(const uint8_t* filename)
 * DESCRIPTION: Opens a file in our filesystem
 * INPUTS   : filename (name of the file we're opening)
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Sets up file descriptor, structure for the file
 */
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
		if(!((pcb->fd_array[i]).flags & ISOLATE_BIT_0)) // If fd is not in-use
			break;
	}

	if(i == MAX_FD_PER_PROCESS) // If there are no open file descriptors
		return FAILURE;

	// Decide whether we're opening RTC, the directory (.), or another file

	if(entry.filetype != 2) // 0: RTC, 1: directory, 2: regular file
		(pcb->fd_array[i]).file_inode = NULL; // Directory and RTC files don't have associated inodes

	else
		(pcb->fd_array[i]).file_inode = get_inode_ptr(entry.inode); // Else, store the inode pointer

	(pcb->fd_array[i]).position = 0; // We start at the beginning of the file
	(pcb->fd_array[i]).flags = 1; // The file descriptor is in use

	// Load the appropriate file operations jump table
	switch(entry.filetype)
	{
		case 0: // case 0, RTC
			(pcb->fd_array[i]).jmp_table = &rtc_table;
			break;
		case 1: // case 1, directory
			(pcb->fd_array[i]).jmp_table = &directory_table;
			break;
		default: // case 2, regular file
			(pcb->fd_array[i]).jmp_table = &file_table;
			break;
	}

	// Call the device-specific open function via jump table
	*((pcb->fd_array[i]).jmp_table->o_func)(filename);

	return i; // Return the file descriptor allocated by open()
}

/*
 * int32_t close_file()
 * DESCRIPTION: Closes a file
 * INPUTS   : fd (file descriptor)
 * OUTPUTS  : none
 * RETURN VALUE: Returns 0 if success, -1 if descriptor is invalid
 * SIDE EFFECTS: Deletes data necessary to handle the file, makes it available to open
*/
asmlinkage int32_t close(int32_t fd)
{
	// Grab esp0 from TSS so that we can access the PCB
	tss_t* tss_base = tss;
	uint32_t esp0 = tss.esp0;

	// Mask bottom 13 bits to get the starting address of the PCB
	// Valid as PCB is at top of kernel stack, which is 8KB-aligned
	pcb_t* pcb = esp0 & MAX_8KB_ALIGNED;

	if(fd < 2 || fd >= MAX_FD_PER_PROCESS)
		return FAILURE; // Don't allow the user to close stdin, stdout, or invalid FDs.

	if((pcb->fd_array[i]).flags == 0) // If the file wasn't opened
		return FAILURE;

	// Else, set the file descriptor is unused
	(pcb->fd_array[i]).flags = 0;

	// Call device-specific close function
	*((pcb->fd_array[i]).jmp_table->c_func)(fd);

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
