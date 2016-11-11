
.data

# Offset to add to ESP to pop arguments from the stack

	POP_ONE_ARG 	= 4
	POP_TWO_ARGS 	= 8
	POP_THREE_ARGS 	= 12
	EBX_MASK = 0x000000FF

.globl syscall_jmp_table # Make this accessible to the syscall interrupt handler

.text

# Jump table used to jump to the correct syscall wrapper
syscall_jmp_table:
	.long 0x0, halt_wrap, execute_wrap, read_wrap
	.long write_wrap, open_wrap, close_wrap, getargs_wrap
	.long vidmap_wrap

# void halt_wrap()
# DESCRIPTION: assembly linkage for the halt system call.
# INPUTS   : EBX: status flags.
# OUTPUTS  : none
# RETURN VALUE: Never actually returns
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
halt_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	andl $EBX_MASK, %ebx
	pushl %ebx # Argument to the halt system call
	call halt

	# Should never get here
	addl $POP_ONE_ARG, %esp
	ret

# void execute_wrap()
# DESCRIPTION: assembly linkage for the execute system call.
# INPUTS   : EBX: string specifying command to execute.
# OUTPUTS  : none
# RETURN VALUE: -1 on failure. 256 if program executed dies via exception. Else, return value is specified by HALT
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
execute_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %ebx # Argument to the execute system call
	call execute
	addl $POP_ONE_ARG, %esp
	ret

# void read_wrap()
# DESCRIPTION: assembly linkage for the read system call.
# INPUTS   : EBX: file descriptor, ECX: pointer to buffer into which to read data, EDX: number of bytes to read
# OUTPUTS  : none
# RETURN VALUE: Number of bytes read, or 0 in special cases.
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
read_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %edx # Arguments to the read system call
	pushl %ecx
	pushl %ebx
	call read
	addl $POP_THREE_ARGS, %esp
	ret

# void write_wrap()
# DESCRIPTION: assembly linkage for the write system call.
# INPUTS   : EBX: file descriptor, ECX: pointer to buffer from which we are writing data, EDX: number of bytes to write
# OUTPUTS  : none
# RETURN VALUE: Number of bytes written, or -1 on failure
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
write_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %edx # Arguments to the write system call
	pushl %ecx
	pushl %ebx
	call write
	addl $POP_THREE_ARGS, %esp
	ret

# void open_wrap()
# DESCRIPTION: assembly linkage for the open system call.
# INPUTS   : EBX: string specifying filename to open.
# OUTPUTS  : none
# RETURN VALUE: -1 on failure, 0 on success
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
open_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %ebx # Argument to the open system call
	call open
	addl $POP_ONE_ARG, %esp
	ret

# void close_wrap()
# DESCRIPTION: assembly linkage for the close system call.
# INPUTS   : EBX: string specifying file descriptor of the file to close.
# OUTPUTS  : none
# RETURN VALUE: -1 on failure, 0 on success
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
close_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %ebx # Argument to the close system call
	call close
	addl $POP_ONE_ARG, %esp
	ret

# void getargs_wrap()
# DESCRIPTION: assembly linkage for the getargs system call.
# INPUTS   : EBX: User-level buffer into which we read command-line arguments. ECX: number of bytes to read
# OUTPUTS  : none
# RETURN VALUE: -1 on failure, 0 on success
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
# The callee are saved by eventual call to a C function
getargs_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %ecx # Arguments to the getargs system call
	pushl %ebx
	call getargs
	addl $POP_TWO_ARGS, %esp
	ret

# void vidmap_wrap()
# DESCRIPTION: assembly linkage for the vidmap system call.
# INPUTS   : EBX: Pointer to the pointer variable storing the user-level video memory mapping
# OUTPUTS  : none
# RETURN VALUE: -1 on failure, 0 on success
# REGISTER USAGE: Need to save caller save registers (except EAX as it stores return value).
vidmap_wrap:
	# No need to construct stack frame as this is just linkage
	# No need to save caller-save registers as this is done in system_call_handler
	# Callee-save registers are handled via calling the C function

	pushl %ebx # Argument to the vidmap system call
	call getargs
	addl $POP_ONE_ARG, %esp
	ret
