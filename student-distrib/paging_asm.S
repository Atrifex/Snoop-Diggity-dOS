.data
	PAGING_ENABLE = 0x80000000
	WP_ENABLE = 0x10000
	FOUR_MB_SIZE_ENABLE = 0x00000010
	ARGUMENT_ONE = 8

.text
.globl paging_hw_enable

# paging_hw_enable(pde_t* base)
# DESCRIPTION: places pointer to the descriptor table into cr3
# 			   sets bit in cr4 that allows to have 4MB pages
#			   sets bit in cr3 that enables paging
# INPUTS   : pde_t* base
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: uses %eax and modifies %cr3, %cr4, and %cr0
paging_hw_enable:
	# sets up stack
	pushl %ebp
	movl %esp, %ebp

	# save callee save registers
	#pushl %ebx
	#pushl %esi
	#pushl %edi

	# assigns the value of pointer to discriptor table
	movl ARGUMENT_ONE(%ebp), %eax
	movl %eax, %cr3

	# enables 4 MB pages
    movl %cr4, %eax
	orl $FOUR_MB_SIZE_ENABLE, %eax
	movl %eax, %cr4

	# clear TLB in the future if needed

	# enables paging
	movl %cr0, %eax
	orl $PAGING_ENABLE, %eax
	#orl $WP_ENABLE, %eax					 # leave this uncommented to enable write protection even in kernel space
	movl %eax, %cr0

	# restore callee save registers
	#popl %edi
	#popl %esi
	#popl %ebx

	leave
	ret

# set_new_page_directory(pde_t* base)
# DESCRIPTION: places pointer to PDE table into CR3
# INPUTS: pde_t* base
# OUTPUTS: none
# RETURN VALUE: none
# REGISTER USAGE: uses %eax, modifies %cr3
# SIDE EFFECTS: changes currently active page directory
set_new_page_directory:
	# set up stack
	pushl %ebp
	movl %esp, %ebp

	# move the page directory address into cr3
	movl ARGUMENT_ONE(%ebp), %eax
	movl %eax, %cr3

	leave
	ret
