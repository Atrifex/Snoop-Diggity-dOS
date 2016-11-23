#include "interrupt_handler.h"


/*
 * void ignore_int_sub()
 * A subroutine used by our null interrupt handler.
 * The null interrupt handler will be called when an unfilled IDT location is referenced,
 * whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays "Unknown interrupt" system message.
*/
void ignore_int_sub()
{
	printf_t("Unknown interrupt");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/* 18 Exceptions */


/*
 * void exception_handler_0_sub()
 * DESCRIPTION: Handler to the divide by zero exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_0_sub()
{
	printf_t("EXCEPTION: DIVIDE BY ZERO ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_2_sub()
 * DESCRIPTION: Handler to the nmi interrupts
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_2_sub()
{
	printf_t("EXCEPTION: NMI INTERRUPT\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_3_sub()
 * DESCRIPTION: Handler to the breakpoints exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_3_sub()
{
	printf_t("EXCEPTION: BREAKPOINT\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_4_sub()
 * DESCRIPTION: Handler to the overflow exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_4_sub()
{
	printf_t("EXCEPTION: OVERFLOW\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_5_sub()
 * DESCRIPTION: Handler to the bound range exceeded exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_5_sub()
{
	printf_t("EXCEPTION: BOUND RANGE EXCEEDED ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_6_sub()
 * DESCRIPTION: Handler to the invalid opcode exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_6_sub()
{
	printf_t("EXCEPTION: INVALID OPCODE ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_7_sub()
 * DESCRIPTION: Handler to the device not available exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_7_sub()
{
	printf_t("EXCEPTION: DEVICE NOT AVAILABLE\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_8_sub()
 * DESCRIPTION: Handler to the double fault exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_8_sub()
{
	printf_t("EXCEPTION: DOUBLE FAULT\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_9_sub()
 * DESCRIPTION: Handler to the coprocessor segment overrun exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_9_sub()
{
	printf_t("EXCEPTION: COPROCESSOR SEGMENT OVERRUN\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_10_sub()
 * DESCRIPTION: Handler to the invalid TSS exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_10_sub()
{
	printf_t("EXCEPTION: INVALID TSS ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_11_sub()
 * DESCRIPTION: Handler to the segment not present exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_11_sub()
{
	printf_t("EXCEPTION: SEGMENT NOT PRESENT ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_12_sub()
 * DESCRIPTION: Handler to the stack-segment fault exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_12_sub()
{
	printf_t("EXCEPTION: STACK-SEGMENT FAULT\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_13_sub()
 * DESCRIPTION: Handler to the general protection fault exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_13_sub()
{
	printf_t("EXCEPTION: GENERAL PROTECTION FAULT\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_14_sub()
 * DESCRIPTION: Handler to the page fault exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_14_sub()
{
	printf_t("EXCEPTION: PAGE FAULT ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_16_sub()
 * DESCRIPTION: Handler to the fpu floating-point exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_16_sub()
{
	printf_t("EXCEPTION: x87 FPU FLOATING-POINT ERROR\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_17_sub()
 * DESCRIPTION: Handler to the alignment check exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_17_sub()
{
	printf_t("EXCEPTION: ALIGNMENT CHECK\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_18_sub()
 * DESCRIPTION: Handler to the machine check exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_18_sub()
{
	printf_t("EXCEPTION: MACHINE CHECK\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}

/*
 * void exception_handler_19_sub()
 * DESCRIPTION: Handler to the SIMD floating-point exception
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void exception_handler_19_sub()
{
	printf_t("EXCEPTION: SIMD FLOATING-POINT EXCEPTION\n");
	halt_excep(EXCEPTION_HALT_RET_VAL);
}



/*
 * 15 total interrupts
 *
 * Master Interrupts
 */

/*
 * void IRQ0_handler_sub()
 * DESCRIPTION: Handler to the Time chip interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ0_handler_sub()
{
	if(in_hardware_int() == TRUE){
		return;
	}

	send_eoi(TIMER_CHIP_LINE_NO);
	round_robin_scheduler();
}

/*
 * void IRQ1_handler_sub()
 * DESCRIPTION: Handler to the Keyboard interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ1_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	process_sent_scancode();
	send_eoi(KEYBOARD_LINE_NO);

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ3_handler_sub()
 * DESCRIPTION: Handler to the COM2 Serial Port interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ3_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: COM2 Serial Port\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ4_handler_sub()
 * DESCRIPTION: Handler to the COM1 Serial Port interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ4_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: COM1 Serial Port\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ5_handler_sub()
 * DESCRIPTION: Handler to the LPT2 interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ5_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: LPT2\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ6_handler_sub()
 * DESCRIPTION: Handler to the Floppy Disk interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ6_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Floppy Disk\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ7_handler_sub()
 * DESCRIPTION: Handler to the LPT1 interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ7_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: LPT1\n");

	// restore flags
	pcb_curr->flags = flags;
}

/* Slave Interrupts */
/*
 * void IRQ8_handler_sub()
 * DESCRIPTION: Handler to the Real Time Clock interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ8_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	//printf_t("IRQ: Real Time Clock\n");	// uncomment for simple test
	//test_interrupts();                // uncomment for more their test
	outb(RTC_CREG_C, RTC_CONTROL_PORT); // selects control register C of the RTC
	inb(RTC_DATA_PORT);

	rtc_flag = 0; // Clear flag used in rtc_read()

	send_eoi(RTC_LINE_NO);

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ9_handler_sub()
 * DESCRIPTION: Handler to the free for peripherals
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ9_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Free for peripherals\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ10_handler_sub()
 * DESCRIPTION: Handler to the free for peripherals
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ10_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Free for peripherals\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ11_handler_sub()
 * DESCRIPTION: Handler to the Eth0 interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ11_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Eth0 (network) \n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ12_handler_sub()
 * DESCRIPTION: Handler to the PS/2 Mouse interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ12_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: PS/2 Mouse\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ13_handler_sub()
 * DESCRIPTION: Handler to the FPU/Coprocessor/Inter-processor interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ13_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: FPU/Coprocessor/Inter-processor\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ14_handler_sub()
 * DESCRIPTION: Handler to the Ide0 interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ14_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Ide0 (hard drive)\n");

	// restore flags
	pcb_curr->flags = flags;
}

/*
 * void IRQ15_handler_sub()
 * DESCRIPTION: Handler to the Ide1 interrupt
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void IRQ15_handler_sub()
{
	// set PCB flag for in hard interrupt
	uint32_t flags;
	tss_t* tss_base = (tss_t*)&tss;
	pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
	flags = pcb_curr->flags;
	pcb_curr->flags |= IN_INTERRUPT_FLAG;

	// set interrupts and call specific handler
	sti();
	printf_t("IRQ: Ide1 (hard drive)\n");

	// restore flags
	pcb_curr->flags = flags;
}
