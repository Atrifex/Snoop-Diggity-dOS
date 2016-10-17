.data
	PAGING_ENABLE = 0x80000000
	WP_ENABLE = 0x10000
	FOUR_MB_SIZE_ENABLE = 0x00000010
	ARGUMENT_ONE = 8

.text
.globl paging_hw_enable

# paging_hw_enable(pde_t* base)
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
