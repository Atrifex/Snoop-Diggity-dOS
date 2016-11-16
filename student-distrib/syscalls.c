#include "syscalls.h"
#include "scheduling.h"

void * execute_jmp_loc;

// local functions
void clear_fd_array(file_info_t * fd_array);


// file operations functions for rtc
static file_operations_t rtc_table = {
    .o_func = open_rtc,
    .c_func = close_rtc,
    .r_func = read_rtc,
    .w_func = write_rtc,
};

// file operations functions for terminal
static file_operations_t terminal_table = {
    .o_func = open_terminal,
    .c_func = close_terminal,
    .r_func = read_terminal,
    .w_func = write_terminal,
};

// file operations functions for regular file
static file_operations_t regular_file_table = {
    .o_func = open_file,
    .c_func = close_file,
    .r_func = read_file,
    .w_func = write_file,
};

// file operations functions for directory
static file_operations_t directory_table = {
    .o_func = open_directory,
    .c_func = close_directory,
    .r_func = read_directory,
    .w_func = write_directory,
};


/*
 * int32_t execute(uint8_t * command)
 * DESCRIPTION: parses input and determines if it is valid. If is valid, then set up paging, stack, and other properties of new process, then start new process by creating a fake iret context and returning
 * INPUTS   : uint8_t * command - pointer to command
 * OUTPUTS  : int32_t - used to check for FAILURE or SUCCESS
 * RETURN VALUE: int32_t - used to check for FAILURE or SUCCESS
 * SIDE EFFECTS: Starts a new process and executes it.
 */
asmlinkage int32_t execute(const uint8_t* command)
{
    // local variables used in execute
	int i;
    unsigned long flags;
	char* argstring = NULL;
	dentry_t entry;
	int32_t result;
	uint32_t proc_memory_start;
    int32_t halt_status;

	int pid;
	char commandstring[MAX_EXECUTE_ARG_SIZE];

	execute_jmp_loc = &&JMP_POS_HALT;

	// get pointer to current pcb by using tss
    tss_t* tss_base = (tss_t*)&tss;
    pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    cli_and_save(flags);

    if(all_pids_available()) {
    	pcb_curr = NULL;
    }

	// if we can't execute since we're out of processes, return failure immediately
	pid = get_available_pid();
	if(pid == FAILURE) return FAILURE;


	// if the command is too long, return failure
	if(strlen((char*) command) >= MAX_EXECUTE_ARG_SIZE){
        return FAILURE;
    }

	// copy the command into a mutable string
	strcpy(commandstring, (char*) command);

	// split "command" by first space character
	// file name will now be in "commandstring", argument string will now be in "argstring"
	for(i = 0; i < strlen(commandstring); i++) {
        argstring = ( commandstring + (i+1) );
		if(commandstring[i] == EMPTY_SPACE) {
			commandstring[i] = NULL_CHAR; // command now ends here
			break;
		}
	}


	// check file validity, load into memory, set up paging, craete PCB/open FD, context switch
	result = read_dentry_by_name((uint8_t*) commandstring, &entry);
	if(result == FAILURE){
        return FAILURE;
    }

	// we can't execute a directory or a device
	if(entry.filetype != FILETYPE_REGULAR){
        return FAILURE;
    }

	// check for executable magic number
	uint8_t magic[NUM_MAGIC_CHECK];
	read_data(entry.inode, 0, magic, sizeof(magic));
	if(!(magic[ELF_ID_BYTE_0] == EXECUTABLE_CHECK_0 && magic[ELF_ID_BYTE_1] == EXECUTABLE_CHECK_1 && magic[ELF_ID_BYTE_2] == EXECUTABLE_CHECK_2 && magic[ELF_ID_BYTE_3] == EXECUTABLE_CHECK_3)) {
        return FAILURE; // not executable
	}

	// read entry point (bytes 24-27 of executable file)
	uint32_t entry_point_address;
	read_data(entry.inode, ENTRY_POINT_INDEX, (uint8_t*) &entry_point_address, sizeof(entry_point_address));

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


    // set up child PCB
    pcb_t* pcb_child = (pcb_t*)(KERNEL_STACK_START - (pid+1)*LITERAL_8KB);
    pcb_child->esp0 = tss_base->esp0;
    pcb_child->parentPCB = pcb_curr;
    pcb_child->pid = pid;
    pcb_child->args = (unsigned char *)argstring;

    // set up kernel stack for child process
    tss_base->esp0 = (uint32_t)(KERNEL_STACK_START - pid*LITERAL_8KB);

    //set stdin in fd array
    pcb_child->fd_array[STDIN].fops_jmp_table = &terminal_table;
    pcb_child->fd_array[STDIN].inode = NULL;
    pcb_child->fd_array[STDIN].position = 0;
    pcb_child->fd_array[STDIN].flags = 1;

    //set stdout in fd array
    pcb_child->fd_array[STDOUT].fops_jmp_table = &terminal_table;
    pcb_child->fd_array[STDOUT].inode = NULL;
    pcb_child->fd_array[STDOUT].position = 0;
    pcb_child->fd_array[STDOUT].flags = 1;

    // clears fd from from 2 to fd 8
    clear_fd_array(pcb_child->fd_array);

	mark_pid_used(pid);
	set_new_page_directory(pd);

    /* IRET Context:
     *       |--------------------|
     *       | .........          |
     *       | EIP/Return Address |
     *       | XCS - user         |
     *       | EFlags             |
     *       | ESP                |
     *       | XSS - user         |
     *       | .........          |
     *       |--------------------|
     */
    unsigned long new_esp = (TASK_VIRTUAL_BASE_ADDRESS + LITERAL_4MB);
    unsigned long new_flags = SET_INTERRUPTS | SET_IOPRIV_USER | SET_PF_RANDBIT;

    pcb_child->esp = get_esp() + ACCOUNT_FOR_RET_ADDR;
    pcb_child->ebp = get_ebp();

    // iret to new program
    iret_to_user((unsigned long)entry_point_address, (unsigned long)USER_CS, (unsigned long)new_flags, (unsigned long)new_esp, (unsigned long)USER_DS);

JMP_POS_HALT:
    // mark pid free and then restore flags
    halt_status = (int32_t)pcb_child->ret_val;

	mark_pid_free(pid);
	restore_flags(flags);

	return halt_status;
}



/*
 * int32_t halt(uint8_t status)
 * DESCRIPTION: halts the currently running process
 * INPUTS   : uint8_t status
 * OUTPUTS  : none
 * RETURN VALUE: returns the value passed in by halt
 * RETURN VALUE: none
 * SIDE EFFECTS: restores the attributes of the parents process and then returns to the process that started the current process
 */
asmlinkage int32_t halt(uint8_t status)
{
	pde_t* pd;
	pcb_t* pcb_parent;

	/****** Restore parent data ******/
	// get pointer to tss and curr pcb
    tss_t* tss_base = (tss_t*)&tss;
    pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    // assign esp0 of parent back into tss
    tss_base->esp0 = pcb_curr->esp0;
	pcb_parent = pcb_curr->parentPCB;

	// restore the PD of the parent
    if(pcb_parent != NULL){
		pd = get_page_directory_for_pid(pcb_parent->pid);
    } else {
		pd = get_kernel_page_directory();
    }
	set_new_page_directory(pd);

    // setting halting status
    pcb_curr->ret_val = (uint32_t)status;

	// restore esp and ebp for the KERNEL
	set_esp_ebp(pcb_curr->esp, pcb_curr->ebp);

	// jump to execute and return back to parent program
	goto *execute_jmp_loc;

	return 0;
}


/*
 * int32_t halt(uint8_t status)
 * DESCRIPTION: halts the currently running process
 * INPUTS   : int32_t status
 * OUTPUTS  : none
 * RETURN VALUE: returns 256 (i.e. the halting program received an exception)
 * RETURN VALUE: none
 * SIDE EFFECTS: restores the attributes of the parents process and then returns to the process that started the current process
 */
int32_t halt_excep(int32_t status)
{
    pde_t* pd;
    pcb_t* pcb_parent;

    /****** Restore parent data ******/
    // get pointer to tss and curr pcb
    tss_t* tss_base = (tss_t*)&tss;
    pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    // assign esp0 of parent back into tss
    tss_base->esp0 = pcb_curr->esp0;
    pcb_parent = pcb_curr->parentPCB;

    // restore the PD of the parent
    if(pcb_parent != NULL){
        pd = get_page_directory_for_pid(pcb_parent->pid);
    } else {
        pd = get_kernel_page_directory();
    }
    set_new_page_directory(pd);

    // setting halting status
    pcb_curr->ret_val = status;

    // restore esp and ebp for the KERNEL
    set_esp_ebp(pcb_curr->esp, pcb_curr->ebp);

    // jump to execute and return back to parent program
    goto *execute_jmp_loc;

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
    pcb_t* pcb = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    // If fd is not in-use, then we can't read
    if(fd >= MAX_FD_PER_PROCESS || fd < 0)
        return ERROR;

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
    pcb_t* pcb = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    // If fd is not in-use, then we can't read
    if(fd >= MAX_FD_PER_PROCESS || fd < 0)
        return ERROR;

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
    pcb_t* pcb = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

	dentry_t entry;

	// Get dentry for this file
	if(read_dentry_by_name(filename, &entry) == FAILURE)
		return FAILURE; // Return -1 if this isn't a valid file

	int i;

	// Allocate an unused file descriptor if possible
	for(i = MIN_FD_PER_PROCESS; i < MAX_FD_PER_PROCESS; i++) // Start at 2 as STDIN and STDOUT are always open
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
 * int32_t close()
 * DESCRIPTION: closes a file
 * INPUTS   : fd (file descriptor)
 * OUTPUTS  : none
 * RETURN VALUE: Returns 0 if success, -1 if descriptor is invalid
 * SIDE EFFECTS: Deletes data necessary to handle the file, makes it available to open
*/
asmlinkage int32_t close(int32_t fd)
{
	// Grab esp0 from TSS so that we can access the PCB
    tss_t* tss_base = (tss_t*)&tss;
    pcb_t* pcb = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    if(fd < MIN_FD_PER_PROCESS || fd >= MAX_FD_PER_PROCESS)
        return FAILURE; // Don't allow the user to close stdin, stdout, or invalid FDs.

	if((pcb->fd_array[fd]).flags == 0) // If the file wasn't opened
		return FAILURE;

	// Else, set the file descriptor is unused
	(pcb->fd_array[fd]).flags = 0;

	// Call device-specific close function
	((pcb->fd_array[fd]).fops_jmp_table->c_func)(fd);

	return 0;
}


/*
 * int32_t getargs() - implemented in checkpoint 4
 * DESCRIPTION: gets the argument supplied to a given process
 * INPUTS   : uint8_t * buf - the buffer to copy agrs into, int32_t num_bytes - number of bytes to copy
 * OUTPUTS  : none
 * RETURN VALUE: Returns 0 if success, -1 if descriptor is invalid
 * SIDE EFFECTS: returns the args in buffer
 */
asmlinkage int32_t getargs(uint8_t* buf, int32_t num_bytes)
{
    if(num_bytes < 0)
        return FAILURE;

    // Grab esp0 from TSS so that we can access the PCB
    tss_t* tss_base = (tss_t*)&tss;
    pcb_t* pcb = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

    int8_t * arguments = (int8_t *)pcb->args;

    if(strlen(arguments) > num_bytes)
        return FAILURE;

    strncpy((int8_t *)buf, arguments, num_bytes);

    return SUCCESS;
    // return FAILURE;
}


/*
 * int32_t vidmap() - implemented in checkpoint 4
 * DESCRIPTION: maps video memory into the memory of a program so that the program can directly write to it.
 * INPUTS   : uint8_t * buf - the buffer to copy agrs into, int32_t num_bytes - number of bytes to copy
 * OUTPUTS  : none
 * RETURN VALUE: Returns 0 if success, -1 if descriptor is invalid
 * SIDE EFFECTS: allows access to video memory for the user
 */
asmlinkage int32_t vidmap(uint8_t** screen_start)
{
    if(screen_start < (uint8_t**)TASK_VIRTUAL_BASE_ADDRESS || screen_start >= (uint8_t**)(TASK_VIRTUAL_BASE_ADDRESS + LITERAL_4MB)){
        return FAILURE;
    }


    *screen_start = (VIDEO);





    return SUCCESS;
}


/*
 * void clear_fd_array()
 * DESCRIPTION: clears the used bit in the fd_array
 * INPUTS   : file_info_t * fd_array - pointer to the current process fd
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: clears value currently store in fd
 */
void clear_fd_array(file_info_t * fd_array)
{
    int i;
    for(i = MIN_FD_PER_PROCESS; i < MAX_FD_PER_PROCESS; i++)
    {
        fd_array[i].fops_jmp_table = NULL;
        fd_array[i].inode = NULL;
        fd_array[i].position = 0;
        fd_array[i].flags = 0;
    }
}
