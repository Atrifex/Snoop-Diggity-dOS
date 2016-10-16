#include "keyboard.h"
#include "lib.h"
#include "i8259.h"

unsigned long keyboard_state = 0;

unsigned long choose_map[NUMB_STATES] = {NORMAL_MAP, SHIFT_MAP, SHIFT_MAP, NORMAL_MAP, CTL_MAP, CTL_MAP, CTL_MAP, CTL_MAP};
unsigned char keyboard_normalmap[NUMB_KEYS] = {}; 
unsigned char keyboard_shiftmap[NUMB_KEYS] =  {}; 
unsigned char keyboard_controlmap[NUMB_KEYS] = { }; 
unsigned char* keyboard_map_select[NUMB_MAPS] = {keyboard_normalmap, keyboard_shiftmap, keyboard_controlmap};

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
	while(1)
	{
		if(a != inb(CONTROL_DATA_PORT)) {
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
	enable_irq(KEYBOARD_LINE_NO);
}

/*
 * display_char
 * DESCRIPTION: returns an unsigned long containing characters to display in the two most significant bits
 *				in the 3 least significant bits returns the (potentially) updated keyboard_state
				1xx control is on, x1x capslock is on, xx1 shift is on
 * INPUT: keyboard_state indicating current keyboard_state of shift, capslock, and control
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
unsigned long display_char()	//shouldn't need to keep track of prev char
{
	unsigned long flags;  //make critical so keyboard_states are able remain correct
	cli_and_save(flags); // save flags and halt interupts for critical section
	char a = get_char();
	// printf("We saw: %x\n", (unsigned long) a);
	restore_flags(flags);	// restore our flags

	unsigned long temp = a;
	return temp;
	/*determines map to use either controlmap, shiftmap, or normal map
	control has highest priority, and therefore goes to control map
	*/
	unsigned long selected_map = choose_map[keyboard_state];
	/*
	switch(a){
		case (CAPS_LOCK_PRESS):
			return (keyboard_state ^ CAP_LOCK_keyboard_state);
		case (CAPS_LOCK_RELEASE):	//dont care about this, return current keyboard_state with null
			return keyboard_state;		
		case (SHIFT_PRESS):
			return (keyboard_state ^ SHIFT_keyboard_state);
		case (SHIFT_RELEASE):
			return (keyboard_state ^ SHIFT_keyboard_state);
		case (CTL_PRESS):
			return (keyboard_state ^ CTL_keyboard_state);
		case (CTL_RELEASE):
			return (keyboard_state ^ CTL_keyboard_state);
			//assuming nothing goes wrong for ctl and shift press sets corresponding keyboard_state bits to 1
			//release sets corresponding keyboard_state bits to 0
		default:
			char adjusted = (keyboard_map_select[selected_map])[a];
			unsigned long adjusted_char = adjusted << CHAR_SHIFT; //
			//adjusted_char += keyboard_state; if not printing in display
			// either print here or return char press encoded in updated keyboard_state
			return keyboard_state; //or adjusted_keyboard_state
	}*/

	
}
