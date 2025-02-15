#ifndef SCHEDULING_H_
#define SCHEDULING_H_

#include "i8259.h"
#include "filesystem.h"
#include "syscalls.h"

#define SHIFT 1
#define BITMASK 0x01
#define FREE_MASK 0xcf
#define START_MEM 0x800000  //8 MB
#define PID_SPACE 0x400000
#define MAX_NUM_PROCS 6

#define CHANNEL_0_PORT 0x0040               // Channel 0 data port (read/write)
#define PIT_CMD_PORT 0x0043 		            // Timer chip command port (write only, used to select a mode etc.)
#define PIT_COMMAND 0x0036		              // Command to send to the timer chip to select correct mode
#define PIT_RELOAD_VAL 35794	            // Frequency = 1193182/PIT_RELOAD_VAL Hertz
#define MASK_UPPER_BYTES 0x000000FF
#define ISOLATE_SECOND_BYTE 8
#define TIMER_CHIP_LINE_NO 0

#define ISOLATE_LEAST_SIG_BIT 0x001

extern int get_available_pid();
extern int mark_pid_used(int pid);
extern int mark_pid_free(int pid);
extern uint32_t block_address_for_process(int pid);
extern uint8_t all_pids_available();
extern uint8_t is_pid_used(int pid);
extern int in_hardware_int();
extern void save_process_context(uint32_t eip, uint32_t esp, uint32_t ebp);
extern void save_and_switch_process_context(int8_t pid);
extern void init_scheduling();
extern void round_robin_scheduler();
extern uint8_t get_terminal_of_current_process();
extern void set_terminal_of_current_process(uint8_t dest_terminal);

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


 /*
 # jmp_to_addr()
 # DESCRIPTION: sets the esp and ebp to a given value
 # INPUTS   : uint32_t par_esp, uint32_t par_ebp
 # OUTPUTS  : none
 # RETURN VALUE: none
 # REGISTER USAGE: esp and ebp
 */
 #define jmp_to_addr(addr) 													                                                 \
     do {                                                                                            \
         asm volatile("\n                                                                            \
                         jmp *%0              \n                                                      \
                      "     	                                                                       \
                      :                                                                              \
                      : "r"(addr)                 				                                           \
             );                                                                                      \
     } while(0)



#endif
