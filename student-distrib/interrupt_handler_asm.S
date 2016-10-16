// .globl exception_handler_0, exception_handler_2, exception_handler_3
// .globl exception_handler_4, exception_handler_5, exception_handler_6, exception_handler_7
// .globl exception_handler_8, exception_handler_9, exception_handler_10, exception_handler_11
// .globl exception_handler_12, exception_handler_13, exception_handler_14
// .globl exception_handler_16, exception_handler_17, exception_handler_18, exception_handler_19
// .globl IRQ0_handler, IRQ1_handler, IRQ3_handler, IRQ4_handler, IRQ5_handler
// .globl IRQ6_handler, IRQ7_handler, IRQ8_handler, IRQ9_handler, IRQ10_handler, IRQ11_handler
// .globl IRQ12_handler, IRQ13_handler, IRQ14_handler, IRQ15_handler 
// .globl system_call_handler
// .globl ignore_int


.globl idt_jmp_table
idt_jmp_table: 
	.long exception_handler_0, ignore_int, exception_handler_2, exception_handler_3
	.long exception_handler_4, exception_handler_5, exception_handler_6, exception_handler_7
	.long exception_handler_8, exception_handler_9, exception_handler_10, exception_handler_11
	.long exception_handler_12, exception_handler_13, exception_handler_14, ignore_int
	.long exception_handler_16, exception_handler_17, exception_handler_18, exception_handler_19
	.long ignore_int, ignore_int, ignore_int, ignore_int, ignore_int, ignore_int, ignore_int
	.long ignore_int, ignore_int, ignore_int, ignore_int, ignore_int
	.long IRQ0_handler, IRQ1_handler, ignore_int, IRQ3_handler, IRQ4_handler, IRQ5_handler
	.long IRQ6_handler, IRQ7_handler, IRQ8_handler, IRQ9_handler, IRQ10_handler, IRQ11_handler
	.long IRQ12_handler, IRQ13_handler, IRQ14_handler, IRQ15_handler 
	.long system_call_handler

.text
# void ignore_int()
# our null interrupt handler. 
# The null interrupt handler will 	be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
ignore_int:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx
	
	# Call subroutine to print
	call ignore_int_sub
	
	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void exception_handler_0()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_0:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli

	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_0_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_2()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_2:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_2_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_3()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_3:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_3_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_4()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_4:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_4_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_5()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_5:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_5_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_6()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_6:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_6_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_7()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_7:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_7_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_8()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_8:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_8_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_0()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_9:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_9_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_10()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_10:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_10_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_11()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_11:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_11_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_12()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_12:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_12_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_13()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_13:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_13_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_14()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_14:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_14_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_16()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_16:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_16_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_0()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_17:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_17_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_18()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_18:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_18_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret


# void exception_handler_19()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
exception_handler_19:
	# we are going to "BSOD" so stop interrupts from flooding screen afterwards
	cli
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call exception_handler_19_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ0_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ0_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ0_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ1_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ1_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ1_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void exception_handler_19()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ3_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ3_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ4_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ4_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ4_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ5_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ5_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ5_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ6_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ6_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ6_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ7_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ7_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ7_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ8_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ8_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ8_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ9_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ9_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ9_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ10_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ10_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ10_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ11_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ11_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ11_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ12_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ12_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ12_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ13_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ13_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ13_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ14_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ14_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ14_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void IRQ15_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
IRQ15_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call IRQ15_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret

# void system_call_handler()
# interrupt handler. 
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers
system_call_handler:
	# save caller registers
	pushl %eax
	pushl %ecx
	pushl %edx

	# Call subroutine to print
	call system_call_handler_sub

	# Restore caller save registers
	popl %edx
	popl %ecx
	popl %eax

	# Return from interrupt
	iret
