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
	printf("Unknown interrupt");
	while(1);
}

/* 18 Exceptions */
void exception_handler_0_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: DIVIDE BY ZERO ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_2_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: NMI INTERRUPT\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_3_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: BREAKPOINT\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_4_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: OVERFLOW\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_5_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: BOUND RANGE EXCEEDED ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_6_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: INVALID OPCODE ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_7_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: DEVICE NOT AVAILABLE\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_8_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: DOUBLE FAULT\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_9_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: COPROCESSOR SEGMENT OVERRUN\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_10_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: INVALID TSS ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_11_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: SEGMENT NOT PRESENT ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_12_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: STACK-SEGMENT FAULT\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_13_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: GENERAL PROTECTION FAULT\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_14_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: PAGE FAULT ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_16_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: x87 FPU FLOATING-POINT ERROR\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_17_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: ALIGNMENT CHECK\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_18_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: MACHINE CHECK\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	

void exception_handler_19_sub()
{
	int ret_address;
	int error_code;

	// When the handler is initially called, the error code is on top of the stack
	// The return address is at ESP+4
	// Since then, we pushed three caller-save, EIP, EBP, and three callee-save (due to C convention)
	// So we need to offset locations by 32
	asm volatile(
		"movl 32(%%esp), %%edx;" 
		"movl 36(%%esp), %%esi;"
		:"S"(ret_address), "D"(error_code)
		: // no inputs
		: "edi", "esi" // clobbers
	);

	printf("EXCEPTION: SIMD FLOATING-POINT EXCEPTION\n");
	printf("Error code: %i\n", error_code);
	printf("Exception occurred at address: %i\n", ret_address);
	while(1);
}	



/* 
 * 15 total interrupts
 *
 * Master Interrupts 
 */
void IRQ0_handler_sub()
{
	printf("IRQ: Timer Chip\n");
}	

void IRQ1_handler_sub()
{
	printf("IRQ: Keyboard\n");
}	

void IRQ3_handler_sub()
{
	printf("IRQ: COM2 Serial Port\n");
}	

void IRQ4_handler_sub()
{
	printf("IRQ: COM1 Serial Port\n");
}	

void IRQ5_handler_sub()
{
	printf("IRQ: LPT2\n");
}	

void IRQ6_handler_sub()
{
	printf("IRQ: Floppy Disk\n");
}	

void IRQ7_handler_sub()
{
	printf("IRQ: LPT1\n");
}	

/* Slave Interrupts */
void IRQ8_handler_sub()
{
	printf("IRQ: Real Time Clock\n");
}	

void IRQ9_handler_sub()
{
	printf("IRQ: Free for peripherals\n");
}	

void IRQ10_handler_sub()
{
	printf("IRQ: Free for peripherals\n");
}	

void IRQ11_handler_sub()
{
	printf("IRQ: Eth0 (network) \n");
}	

void IRQ12_handler_sub()
{
	printf("IRQ: PS/2 Mouse\n");
}	

void IRQ13_handler_sub()
{
	printf("IRQ: FPU/Coprocessor/Inter-processor\n");
}	

void IRQ14_handler_sub()
{
	printf("IRQ: Ide0 (hard drive)\n");
}	

void IRQ15_handler_sub()
{
	printf("IRQ: Ide1 (hard drive)\n");
}	


/* System call */
void system_call_handler_sub()
{
	printf("System Call initiated by User\n");
}	
