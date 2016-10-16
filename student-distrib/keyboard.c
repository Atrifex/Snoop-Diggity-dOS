#include "keyboard.h"

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
	// Resources: http://wiki.osdev.org/%228042%22_PS/2_Controller#Initialising_the_PS.2F2_Controller
	// http://www.computer-engineering.org/ps2keyboard/
	// Assume that there's no USB conflict and that the PS2 controller exists

	// Disable PS2 devices first so they don't send data and mess up initialization
}