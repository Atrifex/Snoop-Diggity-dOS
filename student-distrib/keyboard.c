#include "keyboard.h"

/*
 * get_char
 * DESCRIPTION: Retrieves character from control data port
 * INPUT: none.
 * OUTPUTS: character present in control data port
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
char get_char()
{
	char a = 0;
	while(1){
		if(a != inb(CONTROL_DATA_PORT)){
			a = inb(CONTROL_DATA_PORT);
			if(a != 0)
				return a;
		}
	}
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
	outb(MAKE_RELEASE_WORD, KEYBOARD_PORT);
}
/*
 * display_char
 * DESCRIPTION: returns an unsigned long containing characters to display in the two most significant bits
 *				in the 3 least significant bits returns the (potentially) updated state
				1xx control is on, x1x capslock is on, xx1 shift is on
 * INPUT: state indicating current state of shift, capslock, and control
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
unsigned long display_char(unsigned long state)	//shouldn't need to keep track of prev char
{
	unsigned long flags;  //make critical so states are able remain correct
	cli_and_save(flags); // save flags and halt interupts for critical section
	char a = get_char();
	unsigned long temp = a;
	return temp;
	/*determines map to use either controlmap, shiftmap, or normal map
	control has highest priority, and therefore goes to control map
	*/
	unsigned long selected_map = choose_map[state];
	switch(a){
		case (CAPS_LOCK_PRESS):
			return (state ^ CAP_LOCK_STATE)
		case (CAPS_LOCK_RELEASE):	//dont care about this, return current state with null
			return state;		
		case (SHIFT_PRESS):
			return (state ^ SHIFT_STATE)
		case (SHIFT_RELEASE):
			return (state ^ SHIFT_STATE)
		case (CTL_PRESS):
			return (state ^ CTL_STATE)
		case (CTL_RELEASE):
			return (state ^ CTL_STATE) 
			//assuming nothing goes wrong for ctl and shift press sets corresponding state bits to 1
			//release sets corresponding state bits to 0
		default:
			char adjusted = (keyboard_map_select[selected_map])[a];
			unsigned long adjusted_char = adjusted << CHAR_SHIFT; //
			//adjusted_char += state; if not printing in display
			// either print here or return char press encoded in updated state
			return state; //or adjusted_state
	}
	restore_flags(flags);	// restore our flags
}