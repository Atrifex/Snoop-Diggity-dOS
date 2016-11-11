.data


.text
.globl iret_to_user, get_esp, get_ebp

# paging_hw_enable(pde_t* base)
# DESCRIPTION: places pointer to the descriptor table into cr3
# 			   sets bit in cr4 that allows to have 4MB pages
#			   sets bit in cr3 that enables paging
# INPUTS   : pde_t* base
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: uses %eax and modifies %cr3, %cr4, and %cr0
iret_to_user:
	addl $4, %esp
	iret

get_esp:
	movl %esp, %eax	
	ret

get_ebp:
	movl %ebp, %eax	
	ret
 

