#include "syscalls.h"

static file_operations_t rtc_table = {
    .o_func = open_rtc,
    .c_func = close_rtc,
    .r_func = read_rtc,
    .w_func = write_rtc,
};

static file_operations_t terminal_table = {
    .o_func = open_terminal,
    .c_func = close_terminal,
    .r_func = read_terminal,
    .w_func = write_terminal,
};

static file_operations_t regular_file_table = {
    .o_func = open_file,
    .c_func = close_file,
    .r_func = read_file,
    .w_func = write_file,
};

static file_operations_t directory_table = {
    .o_func = open_directory,
    .c_func = close_directory,
    .r_func = read_directory,
    .w_func = write_directory,
};


asmlinkage int32_t halt(uint8_t status)
{
	return 0;
}

asmlinkage int32_t execute(const uint8_t* command)
{
	int i;
  unsigned long flags;
	char* argstring = NULL;
	dentry_t entry;
	int32_t result;
	int32_t program_length;
	uint32_t proc_memory_start;

	int pid;
	char commandstring[MAX_EXECUTE_ARG_SIZE];

  cli_and_save(flags);

	// if we can't execute since we're out of processes, return failure immediately
	pid = mark_available_pid_used();
	if(pid == FAILURE) return FAILURE;

	// if the command is too long, return failure
	if(strlen((char*) command) >= MAX_EXECUTE_ARG_SIZE){
      mark_pid_free(pid);
      return FAILURE;
  }

	// copy the command into a mutable string
	strcpy(commandstring, (char*) command);

	// split "command" by first space character
	// file name will now be in "commandstring", argument string will now be in "argstring"
	for(i = 0; i < strlen(commandstring); i++) {
		if(commandstring[i] == ' ') {
			commandstring[i] = '\0'; // command now ends here
			argstring = ( commandstring + (i+1) );
			break;
		}
	}


	// check file validity, load into memory, set up paging, craete PCB/open FD, context switch
	result = read_dentry_by_name((uint8_t*) commandstring, &entry);
	if(result == FAILURE){
      mark_pid_free(pid);
      return FAILURE;
  }

	// we can't execute a directory or a device
	if(entry.filetype != FILETYPE_REGULAR){
      mark_pid_free(pid);
      return FAILURE;
  }

	// check for executable magic number
	uint8_t magic[4];
	read_data(entry.inode, 0, magic, sizeof(magic));
	if(! (magic[0] == 0x7f && magic[1] == 0x45 && magic[2] == 0x4c && magic[3] == 0x46)) {
      mark_pid_free(pid);
      return FAILURE; // not executable
	}

	// read entry point (bytes 24-27 of executable file)
	uint32_t entry_point_address;
	read_data(entry.inode, 24, (uint8_t*) &entry_point_address, sizeof(entry_point_address));

	// set up paging for this process
	pde_t* pd = get_page_directory_for_pid(pid);
	pte_t* base_pt = get_base_page_table_for_pid(pid);

	// address we'll load the file to
	proc_memory_start = block_address_for_process(pid);

	// load the file to this address + the offset given
	// load a max of 4MB-offset
	// possibly TODO some error checkign here, but wouldn't be much. 
	read_data(entry.inode, 0, (uint8_t*) ( proc_memory_start + TASK_PROGRAM_IMAGE_OFFSET ), FOUR_MEGS - TASK_PROGRAM_IMAGE_OFFSET);

	// set up page table
	setup_task_paging(pd, base_pt, proc_memory_start);

  // restore interrupts explicitly
  // or modify the bit in the iret context---> that way we dont just do sti when we return back to the parent process

	// set up kernel stack/PCB/TSS
	// IRET/jump/whatever

	restore_flags(flags);

	mark_pid_free(pid);
	return 0;
}

/*
 * int32_t read(int32_t fd, void* buf, int32_t num_bytes)
 * DESCRIPTION: Opens a file in our filesystem
 * INPUTS   : int32_t fd - file descriptor, void * buf - buf read into, int32_t num_bytes - num bytes to read
 * OUTPUTS  : none
 * RETURN VALUE: the number of bytes read or error
 * RETURN VALUE: none
 * SIDE EFFECTS: Sets up file descriptor, structure for the file
 */
asmlinkage int32_t read(int32_t fd, void* buf, int32_t num_bytes)
{
    // Grab esp0 from TSS so that we can access the PCB
    tss_t* tss_base = (tss_t*)&tss;
    uint32_t esp0 = tss_base->esp0;

    // Mask bottom 13 bits to get the starting address of the PCB
    pcb_t* pcb = (pcb_t*)(esp0 & MASK_8KB_ALIGNED);

    // If fd is not in-use, then we can't read
	if(((pcb->fd_array[fd]).flags & ISOLATE_BIT_0) == 0)
        return ERROR;

    // Call the device-specific open function via jump table
    return ((pcb->fd_array[fd]).fops_jmp_table->r_func)(fd, buf, num_bytes);
}

/*
 * int32_t write(int32_t fd, const void* buf, int32_t num_bytes)
 * DESCRIPTION: Opens a file in our filesystem
 * INPUTS   : int32_t fd - file descriptor, const void* buf - buf to read from, int32_t num_bytes - num bytes to write
 * OUTPUTS  :
 * RETURN VALUE: the number of bytes read or error
 * SIDE EFFECTS: Sets up file descriptor, structure for the file
 */
asmlinkage int32_t write(int32_t fd, const void* buf, int32_t num_bytes)
{
    // Grab esp0 from TSS so that we can access the PCB
    tss_t* tss_base = (tss_t*)&tss;
    uint32_t esp0 = tss_base->esp0;

    // Mask bottom 13 bits to get the starting address of the PCB
    pcb_t* pcb = (pcb_t*)(esp0 & MASK_8KB_ALIGNED);

    // If fd is not in-use, then we can't read
	if(((pcb->fd_array[fd]).flags & ISOLATE_BIT_0) == 0)
        return ERROR;

    // Call the device-specific open function via jump table
    return ((pcb->fd_array[fd]).fops_jmp_table->w_func)(fd, buf, num_bytes);
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
	tss_t* tss_base = (tss_t*)&tss;
	uint32_t esp0 = tss_base->esp0;

	// Mask bottom 13 bits to get the starting address of the PCB
	// Valid as PCB is at top of kernel stack, which is 8KB-aligned
	pcb_t* pcb = (pcb_t*)(esp0 & MASK_8KB_ALIGNED);

	dentry_t entry;

	// Get dentry for this file
	if(read_dentry_by_name(filename, &entry) == FAILURE)
		return FAILURE; // Return -1 if this isn't a valid file

	int i;

	// Allocate an unused file descriptor if possible
	for(i = 2; i < MAX_FD_PER_PROCESS; i++) // Start at 2 as STDIN and STDOUT are always open
	{
		if(!((pcb->fd_array[i]).flags & ISOLATE_BIT_0)) // If fd is not in-use
			break;
	}

	if(i == MAX_FD_PER_PROCESS) // If there are no open file descriptors
		return FAILURE;

	// Decide whether we're opening RTC, the directory (.), or another file

	if(entry.filetype != FILETYPE_REGULAR) // 0: RTC, 1: directory, 2: regular file
		(pcb->fd_array[i]).inode = NULL; // Directory and RTC files don't have associated inodes
	else
		(pcb->fd_array[i]).inode = get_inode_ptr(entry.inode); // Else, store the inode pointer

	(pcb->fd_array[i]).position = 0; // We start at the beginning of the file
	(pcb->fd_array[i]).flags = 1; // The file descriptor is in use

	// Load the appropriate file operations jump table
	switch(entry.filetype)
	{
		case FILETYPE_DEVICE: // case 0, RTC
			(pcb->fd_array[i]).fops_jmp_table = &rtc_table;
			break;
		case FILETYPE_DIRECTORY: // case 1, directory
			(pcb->fd_array[i]).fops_jmp_table = &directory_table;
			break;
		case FILETYPE_REGULAR: // case 2, regular file
			(pcb->fd_array[i]).fops_jmp_table = &regular_file_table;
			break;
	}

	// Call the device-specific open function via jump table
	((pcb->fd_array[i]).fops_jmp_table->o_func)(filename);

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
	tss_t* tss_base = (tss_t*)&tss;
	uint32_t esp0 = tss_base->esp0;

	// Mask bottom 13 bits to get the starting address of the PCB
	// Valid as PCB is at top of kernel stack, which is 8KB-aligned
	pcb_t* pcb = (pcb_t*)(esp0 & MASK_8KB_ALIGNED);

	if(fd < 2 || fd >= MAX_FD_PER_PROCESS)
		return FAILURE; // Don't allow the user to close stdin, stdout, or invalid FDs.

	if((pcb->fd_array[fd]).flags == 0) // If the file wasn't opened
		return FAILURE;

	// Else, set the file descriptor is unused
	(pcb->fd_array[fd]).flags = 0;

	// Call device-specific close function
	((pcb->fd_array[fd]).fops_jmp_table->c_func)(fd);

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
