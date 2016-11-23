#include "scheduling.h"

// bit-map to store which pids are in use
uint32_t pid_avail = 0x00;

/*
 * int all_pids_available()
 * DESCRIPTION: checks if all pids are available and reutrn true or false based on that
 * INPUTS   : none
 * OUTPUTS  : "bool" value
 * RETURN VALUE: Returns true "1" if this is the kernel pid. Otherwise, if returns false "0".
 * SIDE EFFECTS: none
 */
uint8_t all_pids_available()
{
	return (pid_avail == 0);
}

/*
 * int get_available_pid()
 * DESCRIPTION: returns the next available pid. Useful for schduling and multiple shells
 * INPUTS   : none
 * OUTPUTS  : int - available pid
 * RETURN VALUE: returns available pid at any given time. If max procs have already been reached, then return FAILURE.
 * SIDE EFFECTS: none
 */
int get_available_pid()
{ // return FAILURE (-1) if none left (MAX_TASKS) reached
	int i;
	uint32_t current = BITMASK;

	for(i = 0; i < MAX_NUM_PROCS; ++i)
	{
		if(!(pid_avail&current))
		{
			return i;
		}

		current = current << SHIFT;
	}
	return FAILURE;
}

/*
 * int mark_pid_used(int pid)
 * DESCRIPTION: Sets a given PID as used
 * INPUTS   : int pid - pid a process wants to use
 * OUTPUTS  : int bool - error state
 * RETURN VALUE: int to show FAILURE or SUCCESS based on if input PID in within PID bounds
 * SIDE EFFECTS: sets a given PID as used
 */
int mark_pid_used(int pid)
{
	if(pid >= 0 && pid < MAX_NUM_PROCS){
		pid_avail = pid_avail | (1 << pid);
		return SUCCESS;
	}
	return FAILURE;
}

/*
 * int mark_pid_free(int pid)
 * DESCRIPTION: Sets a given PID as free
 * INPUTS   : int pid - pid a process wants to free
 * OUTPUTS  : int bool - error state
 * RETURN VALUE: int to show FAILURE or SUCCESS based on if input PID in within PID bounds
 * SIDE EFFECTS: sets a given PID as free
 */
int mark_pid_free(int pid)
{
	if(pid >= 0 && pid < MAX_NUM_PROCS){
		pid_avail = pid_avail & (~(1<<pid));
		return SUCCESS;
	}
	return FAILURE;
}

/*
 * uint8_t is_pid_used(int pid)
 * DESCRIPTION: returns whether or not the given PID is currently in use
 * INPUTS: int pid - PID
 * OUTPUTS: NONE
 * RETURN VALUE: 0 or 1 (true if in use, false if not)
 * SIDE EFFECTS: none
*/
uint8_t is_pid_used(int pid)
{
	if(pid < 0 || pid >= MAX_NUM_PROCS) {
		return FAILURE;
	}

	return (uint8_t) ( ( pid_avail & ( 1 << pid ) ) >> pid );
}

/*
 * int block_address_for_process(int pid)
 * DESCRIPTION: returns physical address to set place the program into
 * INPUTS   : int pid - pid a process wants to use the physical memory for
 * OUTPUTS  : int bool - error state
 * RETURN VALUE: int to show FAILURE or SUCCESS based on if input PID in within PID bounds
 * SIDE EFFECTS: none
 */
uint32_t block_address_for_process(int pid)
{ // 8MB + (4MB*pid)
    uint32_t meme;
    if(pid >= 0 && pid < MAX_NUM_PROCS){
        meme = START_MEM + PID_SPACE*pid;
        return meme;
    }
    return FAILURE;
}

/*
 * int in_hardware_int(int pid)
 * DESCRIPTION: returns physical address to set place the program into
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: returns true if we are in a nested hardware interrupt, else returns false
 * SIDE EFFECTS: none
 */
int in_hardware_int()
{
		// set PCB flag for in hard interrupt
		tss_t* tss_base = (tss_t*)&tss;
		pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

		// currently in hardware interrupt and should return
		if(pcb_curr->flags & IN_INTERRUPT_FLAG){
			return TRUE;
		}else{
			return FALSE;
		}
}



/*
 * void save_process_context(int8_t pid)
 * DESCRIPTION: will save the information related to the current process such as the esp, ebp, eip, etc.
 * INPUTS   : int pid - used to access pcb
 * OUTPUTS  : changes pcb
 * RETURN VALUE: none
 * SIDE EFFECTS: see DESCRIPTION
 */
void save_process_context(uint32_t eip, uint32_t esp, uint32_t ebp)
{
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);

	pcb_curr->eip  = eip;
	pcb_curr->esp_k = esp;
	pcb_curr->ebp_k = ebp;
	get_flags(pcb_curr->eflags);

	return;
}

/*
 * void load_process_context(int8_t pid)
 * DESCRIPTION: used to jump to the process that we want to execute next
 * INPUTS   : int pid - used to access pcb
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: see DESCRIPTION
 */
void save_and_switch_process_context(int8_t pid)
{
		pcb_t* pcb_next;
		tss_t* tss_base = (tss_t*)&tss;

    // set page directory
    set_new_page_directory(get_page_directory_for_pid(pid));

		// save information about currently running process
		save_process_context((uint32_t)&&SWITCH_CONTEXT, get_esp() + ACCOUNT_FOR_RET_ADDR, get_ebp());

		tss_base->esp0 = (KERNEL_STACK_START - (pid)*LITERAL_8KB);

		// set esp, ebp, and flags
		pcb_next = (pcb_t*)(KERNEL_STACK_START - (pid+1)*LITERAL_8KB);
		restore_flags(pcb_next->eflags);
		set_esp_ebp(pcb_next->esp_k, pcb_next->ebp_k);

		jmp_to_addr(pcb_next->eip);

SWITCH_CONTEXT:
		asm("leave;ret");
}



/*
 * void round_robin_scheduler()
 * DESCRIPTION: implements a round robin schduler for the processes running in the three terminals
 * INPUTS   : none
 * OUTPUTS  : switches to next task in list
 * RETURN VALUE: none
 * SIDE EFFECTS: see DESCRIPTION and OUTPUTS
 */
 void round_robin_scheduler()
 {

 }
