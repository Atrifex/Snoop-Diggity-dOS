#include "interrupt_handler.h"
#include "lib.h"
#include "rtc.h"
#include "i8259.h"
#include "keyboard.h"

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
	printf("Unknown interrupt");
	while(1);
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
	clear_and_reset();
	printf("EXCEPTION: DIVIDE BY ZERO ERROR\n");
	while(1);
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
	printf("EXCEPTION: NMI INTERRUPT\n");
	while(1);
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
	printf("EXCEPTION: BREAKPOINT\n");
	while(1);
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
	printf("EXCEPTION: OVERFLOW\n");
	while(1);
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
	printf("EXCEPTION: BOUND RANGE EXCEEDED ERROR\n");
	while(1);
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
	printf("EXCEPTION: INVALID OPCODE ERROR\n");
	while(1);
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
	printf("EXCEPTION: DEVICE NOT AVAILABLE\n");
	while(1);
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
	printf("EXCEPTION: DOUBLE FAULT\n");
	while(1);
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
	printf("EXCEPTION: COPROCESSOR SEGMENT OVERRUN\n");
	while(1);
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
	printf("EXCEPTION: INVALID TSS ERROR\n");
	while(1);
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
	printf("EXCEPTION: SEGMENT NOT PRESENT ERROR\n");
	while(1);
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
	printf("EXCEPTION: STACK-SEGMENT FAULT\n");
	while(1);
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
	printf("EXCEPTION: GENERAL PROTECTION FAULT\n");
	while(1);
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
	printf("EXCEPTION: PAGE FAULT ERROR\n");
	while(1);
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
	printf("EXCEPTION: x87 FPU FLOATING-POINT ERROR\n");
	while(1);
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
	printf("EXCEPTION: ALIGNMENT CHECK\n");
	while(1);
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
	printf("EXCEPTION: MACHINE CHECK\n");
	while(1);
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
	printf("EXCEPTION: SIMD FLOATING-POINT EXCEPTION\n");
	while(1);
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
	printf("IRQ: Timer Chip\n");
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
	// printf("IRQ: Keyboard\n");
	process_sent_scancode();
	send_eoi(KEYBOARD_LINE_NO);
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
	printf("IRQ: COM2 Serial Port\n");
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
	printf("IRQ: COM1 Serial Port\n");
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
	printf("IRQ: LPT2\n");
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
	printf("IRQ: Floppy Disk\n");
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
	printf("IRQ: LPT1\n");
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
	//printf("IRQ: Real Time Clock\n");	// uncomment for simple test
	//test_interrupts();                // uncomment for more their test
	outb(RTC_CREG_C, RTC_CONTROL_PORT); // selects control register C of the RTC
	inb(RTC_DATA_PORT);

	// print 1 for the RTC test for checkpoint 2
    write_terminal(STDOUT, "1", 1);

	rtc_flag = 0; // Clear flag used in rtc_read()

	send_eoi(RTC_LINE_NO);
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
	printf("IRQ: Free for peripherals\n");
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
	printf("IRQ: Free for peripherals\n");
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
	printf("IRQ: Eth0 (network) \n");
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
	printf("IRQ: PS/2 Mouse\n");
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
	printf("IRQ: FPU/Coprocessor/Inter-processor\n");
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
	printf("IRQ: Ide0 (hard drive)\n");
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
	printf("IRQ: Ide1 (hard drive)\n");
}

/* System call */
/*
 * void system_call_handler_sub()
 * DESCRIPTION: system call handler, we will not call this function ever.
 * 				We will need to use a purly asembly function to be a dispatcher.
 *				This function is just a placeholder for now.
 * INPUTS   : inputs are through %eax and %ecx
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays system message.
*/
void system_call_handler_sub()
{
	printf("System Call initiated by User\n");
}
