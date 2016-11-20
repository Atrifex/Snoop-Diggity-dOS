#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "scancodes.h"
#include "types.h"
#include "syscalls.h"
#include "paging.h"

#define CLEAR_SCREEN_SHORTCUT 'l' // control-L clears screen

// terminal masks
#define TERMINAL_ONE_MASK   0x01
#define TERMINAL_TWO_MASK   0x02
#define TERMINAL_THREE_MASK 0x04

// state mask bits
#define SHIFT_MASK 0x1
#define CONTROL_MASK 0x2
#define CAPS_LOCK_MASK 0x4
#define BACKSPACE_MASK 0x8
#define ENTER_MASK 0x10
#define R_SHIFT_MASK 0x20
#define ALT_MASK 0x40

// keyboad buffer attributes
#define KEYBOARD_BUFF_SIZE 128
#define NULL_NL_PADDING 2

// delete
#define DELETE_SCAN_CODE 0x53

// update state macros
#define TURN_SHIFT_ON(state) (state = state | SHIFT_MASK)
#define TURN_R_SHIFT_ON(state) (state = state | R_SHIFT_MASK)
#define TURN_CONTROL_ON(state) (state = state | CONTROL_MASK)
#define TURN_BACKSPACE_ON(state) (state = state | BACKSPACE_MASK)
#define TURN_ENTER_ON(state) (state = state | ENTER_MASK)
#define TURN_ALT_ON(state) (state = state | ALT_MASK)
#define TOGGLE_CAPS_LOCK(state) (state = state ^ CAPS_LOCK_MASK)

#define TURN_SHIFT_OFF(state) (state = state & ~SHIFT_MASK)
#define TURN_R_SHIFT_OFF(state) (state = state & ~R_SHIFT_MASK)
#define TURN_CONTROL_OFF(state) (state = state & ~CONTROL_MASK)
#define TURN_BACKSPACE_OFF(state) (state = state & ~BACKSPACE_MASK)
#define TURN_ENTER_OFF(state) (state = state & ~ENTER_MASK)
#define TURN_ALT_OFF(state) (state = state & ~ALT_MASK)

// check state macros
#define CAPS_LOCK_ON(state) (state & CAPS_LOCK_MASK)
#define CONTROL_ON(state) (state & CONTROL_MASK)
#define BACKSPACE_ON(state) (state & BACKSPACE_MASK)
#define ENTER_ON(state) (state & ENTER_MASK)
#define SHIFT_ON(state) (state & SHIFT_MASK)
#define ALT_ON(state) (state & ALT_MASK)
#define R_SHIFT_ON(state) (state & R_SHIFT_MASK)

// initializer for a multibyte scancode sequence
#define MB_SEQ_INIT 0xE0

// shift value for making a lowercase letter uppercase
#define ASCII_SHIFT_VAL ('a' - 'A')

// keyboard communication packets
#define MAKE_RELEASE_WORD 0xF8
#define KEYBOARD_ACK 0xFA
#define KEYBOARD_RESEND 0xFE

#define PS2_CONTROL_PORT 0x64
#define PS2_DISABLE_KBD 0xAD
#define PS2_ENABLE_KBD 0xAE

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CONTROL_PORT 0x60

#define KEYBOARD_LINE_NO 1

// modifier keys
#define CONTROL_PRESS 0x1D
#define CONTROL_RELEASE 0x9D

#define SHIFT_PRESS 0x2A
#define SHIFT_RELEASE 0xAA

#define R_SHIFT_PRESS 0x36
#define R_SHIFT_RELEASE 0xB6

#define CAPS_LOCK_PRESS 0x3A
#define CAPS_LOCK_RELEASE 0xBA

#define BACKSPACE_PRESS 0x0E
#define BACKSPACE_RELEASE 0x8E

#define ENTER_PRESS 0x1C
#define ENTER_RELEASE 0x9C

#define ALT_PRESS 0x38
#define ALT_RELEASE 0xB8

// attribute related defines
#define ASCII_ONE   49
#define ASCII_TWO   50
#define ASCII_THREE 51
#define ASCII_FOUR  52
#define ASCII_FIVE  53
#define ASCII_SIX   54
#define ASCII_SEVEN 55
#define MAX_ATTRIB 0x0F
#define MIN_ATTRIB 0x01
#define MAX_BACKGROUD_ATTRIB 0x0F
#define MIN_BACKGROUD_ATTRIB 0x00

#define NUM_TERMINALS 3
#define STATE_ONE 1
#define STATE_TWO 2
#define STATE_THREE 3

/* Process the sent scancode after an interrupt */
extern unsigned long process_sent_scancode();
/* Initialize the keyboard device */
extern void init_kbd();

/* Returns the currently active terminal number */
extern uint8_t get_terminal_state();

/* Driver related functions */
extern int32_t open_terminal(const uint8_t *pathname);
extern int32_t read_terminal(int32_t fd, void * buf, int32_t nbytes);
extern int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes);
extern int32_t close_terminal(int32_t fd);

extern void vm_flush_page(pte_t entry);

typedef struct
{
    int screen_x;
    int screen_y;
    // Stdin and index
	uint8_t stdin[KEYBOARD_BUFF_SIZE];   // number of chars in a row is 80 ---> why do we want 128 then?
	int stdin_index;                     // points to current free spot in stdin

	// variables associated with reading
	volatile int allowed_to_read;        // allows read to stop blocking
	volatile int read_waiting;
    uint8_t pid;
} terminal_t;

extern terminal_t terminals[NUM_TERMINALS];

#endif
