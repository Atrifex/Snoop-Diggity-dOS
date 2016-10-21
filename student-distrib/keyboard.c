#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "types.h"

// bit 0 = shift
// bit 1 = control
// bit 2 = caps lock
uint8_t keyboard_state = 0;

uint8_t stdin[KEYBOARD_BUFF_SIZE];       // number of chars in a row is 80 ---> why do we want 128 then?
uint8_t stdin_index;

uint8_t isOpen = 0;

//TODO: make seperate drivers for keybord and terminal

/*
 * get_char
 * DESCRIPTION: Retrieves scancode(s) from control data port
 * INPUT: none.
 * OUTPUTS: scancode(s) present in control data port
 * RETURN VALUE: none.
 * SIDE EFFECTS: Drains keyboard input
*/
uint8_t get_char()
{
    uint8_t data = inb(KEYBOARD_DATA_PORT);

    // get second byte of multi-byte sequences
    if(data == MB_SEQ_INIT) {
    	data = inb(KEYBOARD_DATA_PORT);
    }

    return data;
}

/*
 * init_kbd
 * DESCRIPTION: Initializes the connected PS2 keyboard on boot-up.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
void init_kbd()
{
	// set up the scancode table
    init_scancode_table();

    // init keyboard buffer attributes 
    stdin_index = 0; 
    int i;
    for(i = 0; i < KEYBOARD_BUFF_SIZE - 1; i++)
    {
        stdin[i] = KEYBOARD_EMPTY_SPACE;
    }
    stdin[KEYBOARD_EMPTY_SPACE-1] = NULL_CHAR;

    //TODO: try to change the mode of the keyboard

    // enable the interrupt on the PIC
    enable_irq(KEYBOARD_LINE_NO);
}

/*
 * open_kbd
 * DESCRIPTION: opens file to kbd
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: opens file to kbd inptut
*/
int open_terminal()
{
    isOpen = 1;
    return 0;
}



/*
 * close_kbd
 * DESCRIPTION: opens file to kbd
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: opens file to kbd inptut
*/
int close_terminal()
{
    isOpen = 0;
    return;
}


/*
 * read_kbd
 * DESCRIPTION: gets pointer to input buffer
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: opens file to kbd inptut
*/
ssize_t read_terminal()
{
    if(isOpen)
    {
        
    }
    else
    {
        
    }
}

/*
 * write_kbd
 * DESCRIPTION: does nothing
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: does nothing
*/
ssize_t write_terminal()
{

}


/*
 * display_char
 * DESCRIPTION: returns an unsigned long containing characters to display in the two most significant bits
 *                              in the 3 least significant bits returns the (potentially) updated keyboard_state
 *                              1xx control is on, x1x capslock is on, xx1 shift is on
 * INPUT: keyboard_state indicating current keyboard_state of shift, capslock, and control
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
unsigned long process_sent_scancode()
{
    scancode_t mapped;
    uint8_t raw_scancode = get_char();

    /*determines map to use either controlmap, shiftmap, or normal map
    control has highest priority, and therefore goes to control map
    */

    // see if we need to update state
    switch(raw_scancode) {
        case (CAPS_LOCK_PRESS):
            TOGGLE_CAPS(keyboard_state);
            break;
        case (CAPS_LOCK_RELEASE):
            //don't care about this
            break;
        case (SHIFT_PRESS):
            TOGGLE_SHIFT(keyboard_state);
            break;
        case (SHIFT_RELEASE):
            TOGGLE_SHIFT(keyboard_state);
            break;
        case (CONTROL_PRESS):
            TOGGLE_CONTROL(keyboard_state);
            break;
        case (CONTROL_RELEASE):
            TOGGLE_CONTROL(keyboard_state);
            break;
    }

	mapped = scancode_table[raw_scancode];

	if(!IS_MAKE_SC(mapped)) {
		return keyboard_state;
	}

    
	if(CONTROL_ON(keyboard_state)) {
		if(mapped.result == CLEAR_SCREEN_SHORTCUT) {
			clear_and_reset();
		}
	} else if(CAPS_LOCK_ON(keyboard_state) && !SHIFT_ON(keyboard_state)) {
		if(IS_LETTER_SC(mapped)) {
			putc(mapped.result - ASCII_SHIFT_VAL);
		} else if(IS_PRINTABLE_SC(mapped)) {
			putc(mapped.result); // general printable characters unaffected by caps lock
		}
	} else if(SHIFT_ON(keyboard_state)) {
		if(IS_LETTER_SC(mapped) && !CAPS_LOCK_ON(keyboard_state)) {
			putc(mapped.result - ASCII_SHIFT_VAL); // shifting letters is simple
		} else if(IS_PRINTABLE_SC(mapped)) {
			if(non_alpha_shift_table[mapped.result] != 0) {
				putc(non_alpha_shift_table[mapped.result]);
			} else {
				putc(mapped.result); // general printable characters unaffected by caps lock
			}
		}
	} else {
		if(IS_PRINTABLE_SC(mapped)) {
			putc(mapped.result);
		}
	}
    
    return keyboard_state; // return current kb state
}
