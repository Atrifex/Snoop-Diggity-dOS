#include "interrupt_handler.h"

/*
 * void ignore_int_sub()
 * A subroutine used by our null interrupt handler. 
 * The null interrupt handler will be called when an unfilled IDT location is referenced,
 * whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays "Unknown interrupt" system message.
*/
void ignore_int_sub()
{
	printf("Unknown interrupt");
}