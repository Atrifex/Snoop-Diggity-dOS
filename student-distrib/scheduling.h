#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include "filesystem.h"
#include "syscalls.h"

#define SHIFT 1
#define BITMASK 0x01
#define FREE_MASK 0xcf
#define START_MEM 0x800000 //8 MB
#define PID_SPACE 0x400000
#define MAX_NUM_PROCS 6

extern int get_available_pid();
extern int mark_pid_used(int pid);
extern int mark_pid_free(int pid);
extern uint32_t block_address_for_process(int pid);
extern uint8_t all_pids_available();
extern uint8_t is_pid_used(int pid);
extern int in_hardware_int();
extern void save_process_context(int8_t pid);
extern void load_process_context(int8_t pid);

/*
 * Task 1
 * | Time Chip (dont switch)                                  |
 * | IRQ (save state, clear flag, perform work, restore flag) |
 * | IRQ (save state, clear flag, perform work, restore flag) |
 * | sys call (set flag)                                      |
 *
 * Task 1
 * | Time Chip (switch using in place IRET) |
 * | sys call (set flag)                    |
 *
 * Task 2
 * | Time Chip (switch using in place IRET) |
 *
 */


#endif
