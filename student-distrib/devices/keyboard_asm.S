
.data

# Offset to add to ESP to pop arguments from the stack

#define ARGUMENT_ONE 8

# the jump table that contains pointers to all of the functions
.text
.globl vm_flush_page


# vm_flush_page(pte_t* base)
# DESCRIPTION: flushes a single address from the page table
# INPUTS: pte_t entry
# OUTPUTS: none
# RETURN VALUE: none
# REGISTER USAGE: uses %eax
# SIDE EFFECTS: see DESCRIPTION
vm_flush_page:
	pushl %ebp
	movl %esp, %ebp

	movl	ARGUMENT_ONE(%ebp),%eax
	invlpg	(%eax)

	leave
	ret

