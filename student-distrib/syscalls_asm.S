.data
    LOCAL_RET_VALUE = 4

.text
.globl iret_to_user, get_esp, get_ebp

# iret_to_user((unsigned long)entry_point_address, (unsigned long)USER_CS, (unsigned long)new_flags, (unsigned long)new_esp, (unsigned long)USER_DS);
# DESCRIPTION: Caller pushes iret context such that this function can call iret.
# INPUTS   : iret context
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: none
iret_to_user:
	addl $LOCAL_RET_VALUE, %esp
	iret


# uint32_t get_esp()
# DESCRIPTION: returns the value of esp
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: returns the value of esp
# REGISTER USAGE: eax for return
get_esp:
	movl %esp, %eax
	ret


# uint32_t get_ebp()
# DESCRIPTION: returns the value of ebp
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: returns the value of ebp
# REGISTER USAGE: eax for return
get_ebp:
	movl %ebp, %eax
	ret


