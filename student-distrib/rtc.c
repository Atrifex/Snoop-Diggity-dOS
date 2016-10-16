#include "rtc.h"
#include "lib.h"
#include "i8259.h"

// registers declared with nonmaskable interrupts


/*
# void init_rtc()
# initializes RTC 
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

    // save flags and halt interupts for critical section
	cli_and_save(flags);

    // select register B and get current state of register to create desired state
	outb(RTC_CREG_B, RTC_CONTROL_PORT);
	char portdata = inb(RTC_DATA_PORT) | ENABLE_PERIODIC_INT;

    // select register B and update it with PIE enabled
	outb(RTC_CREG_B, RTC_CONTROL_PORT);
	outb(portdata, RTC_DATA_PORT);

    // select register A and set interrupt handler rate to 2 while maintianing significant bits
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	portdata = (inb(RTC_DATA_PORT) & MAINTAIN_SIG_BYTE) | DESIRED_PIE_RATE;

    // select register A and update its value 
	outb(RTC_CREG_A, RTC_CONTROL_PORT);
	outb(portdata, RTC_DATA_PORT);

    // restore flags
	restore_flags(flags);

    // enable irqs so taht we can get interrupts from the RTC
	enable_irq(RTC_LINE_NO);
}
