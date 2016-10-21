#ifndef RTC_H
#define RTC_H

/* port data */
#define RTC_CONTROL_PORT 0x70
#define RTC_DATA_PORT 0x71

/* The below are used to select control registers with NMI disabled */
#define RTC_CREG_A 0x8A
#define RTC_CREG_B 0x8B
#define RTC_CREG_C 0x8C
#define RTC_CREG_C_NO_NMI 0x0C

/* words written to the RTC */
#define ENABLE_PERIODIC_INT 0x40
#define MAINTAIN_SIG_BYTE 0xF0
#define DESIRED_PIE_RATE 0x0F
#define RTC_LINE_NO 8

// RTC interrupt frequency min and max
#define RTC_FREQ_MIN 2
#define RTC_FREQ_MAX 1024
#define FREQ_FORMULA_VAL 32768

/* used in kernel.c to init the rtc */
extern void init_rtc();

/*Called by read system call when RTC is specified */
extern int read_rtc(int32_t fd, void* buf, int32_t nbytes);

// Called by write system call when RTC is specified
extern int write_rtc(int freq);

#endif
