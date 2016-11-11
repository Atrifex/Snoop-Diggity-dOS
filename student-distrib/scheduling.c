#include "scheduling.h"
#define SHIFT 1
#define BITMASK 0x1
#define FREE_MASK 0xcf
#define START_MEM 0x800000 //8 MB
#define PID_SPACE 0x400000

uint8_t PID_AVAIL = 0x00; 

int get_available_pid() { // return FAILURE (-1) if none left (MAX_TASKS) reached
	int i;
	int8_t current = BITMASK;
	for(i = 0; i < 6; ++i) {
		if(!(PID_AVAIL&current))
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
	if(pid > 6 || pid < 1){
		for(i = 1; i < pid; ++i){
			mask = mask << SHIFT;
		}
		PID_AVAIL = PID_AVAIL | mask;
		return SUCCESS;
	}
	return FAILURE;
}

int mark_pid_free(int pid) {
	int i;
	int8_t mask = BITMASK;
	if(pid > 6 || pid < 1){
		for(i = 1; i < pid; ++i){
			mask = mask << SHIFT;
		}
		mask = mask ^ FREE_MASK;
		PID_AVAIL = PID_AVAIL & mask;
		return SUCCESS;
	}
	return FAILURE;

}

uint32_t block_address_for_process(int pid) { // 8MB + (4MB*pid)
    if(pid >= 0 && pid < 6){
        uint32_t meme = START_MEM + PID_SPACE*pid;
        return meme;
    }
    return FAILURE;
}
