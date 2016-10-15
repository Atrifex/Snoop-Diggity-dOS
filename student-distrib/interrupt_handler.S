#include "interrupt_handler.h"

# void ignore_int()
# our null interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# SIDE EFFECTS: Displays "Unknown interrupt" system message.
ignore_int:
	# Save ALL registers

	# Call subroutine to print

	# Restore all registers

	# Return from interrupt
	iret