#define RCT_CONTROL_PORT 0x70
#define RTC_DATA_PORT 0x71
/* The below are used to select control registers with NMI disabled*/
#define RCT_CREG_A 0x8A
#define RCT_CREG_B 0x8B
#define RCT_CREG_C 0x8C
#define ENABLE_PERIODIC_INT x40
#define MAINTAIN_SIG_BYTE xF0
#define DESIRED_PIE_RATE x02

// registers declared with nonmaskable interrupts


# void init_rtc()
# initializes RTC 
# Initialized rtc registers by selecting the periodic interupt enable bit to true and altering the
# Rate which is indicated by the least significant nibble of register A 
# Should be called upon initialization of the kernel
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers and modify control registers A and B of the RTC

void init_rtc(){
	unsigned long flags; 
	cli_and_save(flags); // save flags and halt interupts for critical section
	outb(RTC_CONTROL_PORT, RCT_CREG_B); selects control register B of the RTC
	char portdata = inb(RTC_DATA_PORT) | ENABLE_PERIODIC_INT; //get current state and set the disired rate
	outb(RTC_DATA_PORT, portdata); //update register B with PIE enabled
	outb(RTC_CONTROL_PORT, RCT_CREG_A); selects control register A of the RTC
	portdata = (inb(RTC_DATA_PORT) & MAINTAIN_SIG_BYTE) | DESIRED_PIE_RATE; //Set interrupt handle rate to 2 while maintaining significant bits
	outb(RTC_DATA_PORT, portdata); //updatew register B with PIE enabled
	restore_flags(flags);	// restore our flags
}