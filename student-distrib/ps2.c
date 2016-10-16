#include "ps2.h"
#include "lib.h"


#if USE_PS2_CONTROLLER
/*
 * init_ps2
 * DESCRIPTION: Initializes the PS2 controller on boot-up. For using both keyboard and mouse.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the PS2 controller
*/
void init_ps2()
{
	// Resources: http://wiki.osdev.org/%228042%22_PS/2_Controller#Initialising_the_PS.2F2_Controller
	// http://www.computer-engineering.org/ps2keyboard/
	// Assume that there's no USB conflict and that the PS2 controller exists
	// Assume that keyboard is on the first channel, mouse is on second channel

	// @TODO: Add locks around writes to shared port when we get lock API.

	int ps2_status, ps2_data, ps2_config;
	int dual_channel;

	// Disable PS2 devices first so they don't send data and mess up initialization
	outb(DISABLE_KBD, PS2_STATUS_CMD_PORT); // disable keyboard
	outb(DISABLE_MOUSE, PS2_STATUS_CMD_PORT); // disable mouse (maybe unnecessary right now)

	// Wait until keyboard communication is inhibited

	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(ps2_status & ISOLATE_BIT_4) // 0: keyboard is disabled, 1: it's enabled
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}

	// Flush the output buffer by reading from the input buffer
	inb(PS2_BUFFER_PORT);

	// Set controller configuration byte (don't need to test OBF as we know output buffer is flushed)
	
	outb(READ_CMD_BYTE, PS2_STATUS_CMD_PORT); // Try to read the previous value

	// Wait for input buffer to be readable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(!(ps2_status & ISOLATE_BIT_1)) // 0: can't read, 1: can read
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}
	
	ps2_config = inb(PS2_BUFFER_PORT); // Get previous value

	dual_channel = ps2_config & ISOLATE_BIT_5; // 0: single channel (only keyboard), 1: dual channel (keyboard and mouse)

	// Disable IRQs and disable translation
	ps2_config &= CLEAR_BIT_0;
	ps2_config &= CLEAR_BIT_1;
	ps2_config &= CLEAR_BIT_6;

	// Wait for output buffer to be writeable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(ps2_status & ISOLATE_BIT_0) // 0: okay to write, 1: can't write
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}

	outb(WRITE_CMD_BYTE, PS2_STATUS_CMD_PORT); // Write it back (command)
	outb(ps2_config, PS2_BUFFER_PORT); // Parameters are taken on port 0x60 and written after we send the command

	// Wait for output buffer to be writeable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(ps2_status & ISOLATE_BIT_0) // 0: okay to write, 1: can't write
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}

	// Perform controller self test
	outb(CONTROLLER_SELF_TEST, PS2_STATUS_CMD_PORT);

	// Wait for input buffer to be readable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(!(ps2_status & ISOLATE_BIT_1)) // 0: can't read, 1: can read
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}
	
	ps2_data = inb(PS2_BUFFER_PORT); // Get response to command

	if(ps2_data != CONTROLLER_OKAY_RESPONSE)
	{
		printf("Error! PS2 controller initialization failed\n");
		while(1); // Maybe change later
	}

	// Skip step 7 (it seems redundant)

	// Keyboard interface test
	outb(KEYBOARD_INTERFACE, PS2_STATUS_CMD_PORT);

	// Wait for input buffer to be readable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(!(ps2_status & ISOLATE_BIT_1)) // 0: can't read, 1: can read
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}
	
	ps2_data = inb(PS2_BUFFER_PORT); // Get response to command

	switch(ps2_data) // 0: keyboard is okay. 1: error code returned
	{
		case 1:
			printf("Error! Keyboard clock line stuck low\n");
			while(1);
		case 2:
			printf("Error! Keyboard clock line stuck high\n");
			while(1);
		case 3:
			printf("Error! Keyboard data line stuck low\n");
			while(1);
		case 4:
			printf("Error! Keyboard data line stuck high\n");
			while(1);
		default:
			break;
	}

	if(dual_channel) // If we have a mouse
	{
		// Mouse interface test
		outb(MOUSE_INTERFACE, PS2_STATUS_CMD_PORT);

		// Wait for input buffer to be readable
		ps2_status = inb(PS2_STATUS_CMD_PORT);
		while(!(ps2_status & ISOLATE_BIT_1)) // 0: can't read, 1: can read
		{
			ps2_status = inb(PS2_STATUS_CMD_PORT);
		}
		
		ps2_data = inb(PS2_BUFFER_PORT); // Get response to command

		switch(ps2_data) // 0: mouse is okay. 1: error code returned
		{
			case 1:
				printf("Error! Mouse clock line stuck low\n");
				while(1);
			case 2:
				printf("Error! Mouse clock line stuck high\n");
				while(1);
			case 3:
				printf("Error! Mouse data line stuck low\n");
				while(1);
			case 4:
				printf("Error! Mouse data line stuck high\n");
				while(1);
			default:
				break;
		}

	}

	// Enable IRQs on both channels

	// Set controller configuration byte
	
	outb(READ_CMD_BYTE, PS2_STATUS_CMD_PORT); // Try to read the previous value

	// Wait for input buffer to be readable
	ps2_status = inb(PS2_STATUS_CMD_PORT);
	while(!(ps2_status & ISOLATE_BIT_1)) // 0: can't read, 1: can read
	{
		ps2_status = inb(PS2_STATUS_CMD_PORT);
	}
	
	ps2_config = inb(PS2_BUFFER_PORT); // Get previous value

	// Enable bits for IRQs
	ps2_config |= SET_BIT_0; // keyboard
	if(dual_channel)
		ps2_config |= SET_BIT_1; // mouse if necessary

	// Enable devices
	outb(ENABLE_KBD, PS2_STATUS_CMD_PORT); // keyboard
	if(dual_channel)
		outb(ENABLE_MOUSE, PS2_STATUS_CMD_PORT); // mouse if necessary

	// Final step: reset devices by sending RESET command (0xFF) to keyboard/mouse
	// Need to figure out how to specify one versus the other
}
#endif