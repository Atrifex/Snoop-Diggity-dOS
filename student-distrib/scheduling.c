#include "scheduling.h"


uint8_t pid_avail = 0x00;

int get_available_pid() { // return FAILURE (-1) if none left (MAX_TASKS) reached
	int i;
	int8_t current = BITMASK;
	for(i = 0; i < 6; ++i) {
		if(!(pid_avail&current))
			{
				return i;
			}
			current = current << SHIFT;
	}
	return FAILURE;
}

int mark_pid_used(int pid) {
	int i;
	int8_t mask = BITMASK;
	if(pid >= 0 && pid < 6){
		for(i = 0; i < pid; ++i){
			mask = mask << SHIFT;
		}
		pid_avail = pid_avail | mask;
		return SUCCESS;
	}
	return FAILURE;
}

int mark_pid_free(int pid) {
	int i;
	int8_t mask = BITMASK;
	if(pid >= 0 && pid < 6){
		for(i = 0; i < pid; ++i){
			mask = mask << SHIFT;
		}
		mask = mask ^ FREE_MASK;
		pid_avail = pid_avail & mask;
		return SUCCESS;
	}
	return FAILURE;

}

uint32_t block_address_for_process(int pid) { // 8MB + (4MB*pid)
    uint32_t meme;
    if(pid >= 0 && pid < 6){
        meme = START_MEM + PID_SPACE*pid;
        return meme;
    }
    return FAILURE;
}

