#include "rtc.h"
#include "lib.h"
#include "i8259.h"

// TODO: Change open() and close() once we've loaded the filesystem.
// And again for MP3.3

// File-scope flag: is the RTC open?
static int rtc_enable = 0;

// Shared flag with interrupt_handler.c
extern volatile int rtc_flag;

/*
# void init_rtc()
# DESCRIPTION: initializes RTC
# Initialized rtc registers by selecting the periodic interupt enable bit to true and altering the
# Rate which is indicated by the least significant nibble of register A
# Should be called upon initialization of the kernel
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers and modify control registers A and B of the RTC
*/
void init_rtc()
{
	unsigned long flags;

    // save flags and halt interrupts for critical section
	cli_and_save(flags);

    // select register B and get current state of register to create desired state
	outb(RTC_CREG_B, RTC_CONTROL_PORT);
	char portdata = inb(RTC_DATA_PORT) | ENABLE_PERIODIC_INT;

    // select register B and update it with PIE enabled
	outb(RTC_CREG_B, RTC_CONTROL_PORT);
	outb(portdata, RTC_DATA_PORT);

    // select register A and set interrupt handler rate to 2 while maintaining significant bits
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	portdata = (inb(RTC_DATA_PORT) & MAINTAIN_SIG_BYTE) | DESIRED_PIE_RATE;

    // select register A and update its value
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	outb(portdata, RTC_DATA_PORT);

    // restore flags
	restore_flags(flags);

    // enable irqs so that we can get interrupts from the RTC
	enable_irq(RTC_LINE_NO);
	rtc_enable = 1;
}

/*
# int read_rtc()
# DESCRIPTION: Waits for an RTC interrupt (since there isn't actually anything to read)
# Should be called by the read() system call when the RTC device file is specified
# INPUTS   : fd, buf, nbytes (all ignored)
# OUTPUTS  : none
# RETURN VALUE: Always returns 0 as we never read any bytes
# SIDE EFFECTS: Waits for an interrupt from the RTC
*/
int read_rtc(int32_t fd, void* buf, int32_t nbytes)
{
	if(!rtc_enable)
		return -1;

	rtc_flag = 1; // Set a flag and wait for interrupt handler to clear it

	while(rtc_flag != 0);

	// printf("Read RTC\n");

	return 0; // 0 bytes were read
}

/*
# int write_rtc(int freq)
# DESCRIPTION: Writes a 4-byte integer (representing the interrupt rate in Hertz) to the RTC 
# Should be called by the write() system call when the RTC device file is specified
# INPUTS   : freq (RTC interrupt rate)
# OUTPUTS  : none
# RETURN VALUE: Returns the number of bytes written, or -1 if the write failed
# SIDE EFFECTS: (Potentially) changes the RTC's interrupt rate
*/
int write_rtc(int freq)
{
	if(!rtc_enable)
		return -1;

	unsigned long flags;

	// Input checking
	// RTC interrupt rate must be a power of 2 in range [2, 1024]
	// The last check relies on the fact that powers of 2 are 100...00 and one less than this is 011...11
	int rtc_cond = (freq >= RTC_FREQ_MIN) && (freq <= RTC_FREQ_MAX) && !(freq & (freq - 1));

	if(!rtc_cond)
		return -1; // Return error

	// Else, change the frequency
	// Use formula: freq = 32768 >> (rate - 1)
	int i = FREQ_FORMULA_VAL/freq;
	int rate = 1;

	while(i != 1)
	{
		i >>= 1;
		rate++; // At the end of the while loop, "rate" will store our desired rate
	}

	// save flags and halt interrupts for critical section
	cli_and_save(flags);

	// select register A and set interrupt handler rate to the user-specified value
	// while maintaining significant bits
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	unsigned char portdata = (inb(RTC_DATA_PORT) & MAINTAIN_SIG_BYTE) | rate;

    // select register A and update its value
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	outb(portdata, RTC_DATA_PORT);

    // restore flags
	restore_flags(flags);

    // enable irqs so that we can get interrupts from the RTC
	enable_irq(RTC_LINE_NO);

	return 1; // We're writing one byte as register A is 8 bits
}

/*
# int open_rtc()
# DESCRIPTION: Opens the RTC (calls RTC_init)
# INPUTS   : none FOR NOW
# OUTPUTS  : none
# RETURN VALUE: Returns 0 if success, -1 if no descriptors are free
# SIDE EFFECTS: Sets up data necessary to handle the RTC file
*/
int open_rtc()
{
	// TODO: For CP3, allocate the file descriptor

	init_rtc(); // Set interrupt rate to 2 Hz by default
	rtc_enable = 1; // Open and close will work now
	return 0;
}

/*
# int open_rtc()
# DESCRIPTION: Closes the RTC
# INPUTS   : fd (RTC file descriptor)
# OUTPUTS  : none
# RETURN VALUE: Returns 0 if success, -1 if descriptor is invalid
# SIDE EFFECTS: Deletes data necessary to handle the RTC file
*/
int close_rtc(int32_t fd)
{
	// TODO: For CP3, close the file descriptor

	rtc_enable = 0;
	return 0;
}
