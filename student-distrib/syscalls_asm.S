.data


.text
.globl iret_to_user

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
