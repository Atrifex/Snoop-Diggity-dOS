#include "interrupt_handler.h"

# Make interrupt handlers visible so we can fill IDT
.globl ignore_int

# void ignore_int()
# our null interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
ignore_int:
	# Save ALL registers except eip (should be taken care of by iret)
	pushad # general-purpose registers
	pushfd # flags

	# Call subroutine to print
	call do_ignore_int

	# Restore all registers
	popfl # flags
	popfd # general-purpose registers

	# Return from interrupt
	iret

# void divide_error()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
divide_error:
	# Save ALL registers except eip (should be taken care of by iret)
	pushad # general-purpose registers
	pushfd # flags

	# Call subroutine to print
	call do_divide_error

	# Restore all registers
	popfl # flags
	popfd # general-purpose registers

	# Return from interrupt
	iret

debug:

nmi:

