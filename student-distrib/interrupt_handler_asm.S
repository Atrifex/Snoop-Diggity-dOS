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

# the jump table that contains pointers to all of the functions
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
# The null interrupt handler will be called when an unfilled IDT location is referenced,
# whether due to a kernel issue or a hardware issue. Basically, it should never be called.
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to exception_handler_0_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_0:
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
# DESCRIPTION: wrapper to exception_handler_2_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_2:
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
# DESCRIPTION: wrapper to exception_handler_3_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_3:
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
# DESCRIPTION: wrapper to exception_handler_4_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_4:
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
# DESCRIPTION: wrapper to exception_handler_5_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_5:
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
# DESCRIPTION: wrapper to exception_handler_6_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_6:
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
# DESCRIPTION: wrapper to exception_handler_7_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_7:
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
# DESCRIPTION: wrapper to exception_handler_8_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_8:
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
# DESCRIPTION: wrapper to exception_handler_9_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_9:
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
# DESCRIPTION: wrapper to exception_handler_10_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_10:
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
# DESCRIPTION: wrapper to exception_handler_11_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_11:
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
# DESCRIPTION: wrapper to exception_handler_12_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_12:
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
# DESCRIPTION: wrapper to exception_handler_13_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_13:
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
# DESCRIPTION: wrapper to exception_handler_14_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_14:
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
# DESCRIPTION: wrapper to exception_handler_16_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_16:
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
# DESCRIPTION: wrapper to exception_handler_17_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_17:
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
# DESCRIPTION: wrapper to exception_handler_18_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_18:
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
# DESCRIPTION: wrapper to exception_handler_19_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
exception_handler_19:
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
# DESCRIPTION: wrapper to IRQ0_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ1_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ3_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ4_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ5_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ6_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ7_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ8_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ9_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ10_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ11_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ12_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ13_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ14_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper to IRQ15_handler_sub. Allows for IRET to be used
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: we save caller save registers and the callee are saved by functoin call
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
# DESCRIPTION: wrapper for our system call handlers. Allows for IRET to be used
# INPUTS   : EAX: call number. EBX, ECX, EDX: arguments.
# OUTPUTS  : none
# RETURN VALUE: --1 on failure. Others indicate success (meaning varies by call)
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
system_call_handler:
	# save caller registers
	pushl %ecx
	pushl %edx

	# Call the appropriate syscall assembly wrapper based on the syscall number
	call *syscall_jmp_table(, %eax, 4) # EAX stores the call number

	# Restore caller save registers
	popl %edx
	popl %ecx

	# Return from interrupt
	iret
