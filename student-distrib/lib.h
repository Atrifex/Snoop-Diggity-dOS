/* lib.h - Defines for useful library functions
 * vim:ts=4 noexpandtab
 */

#ifndef _LIB_H
#define _LIB_H

#include "types.h"

#define VGA_CURSOR_LOW_BIT_REG  0x0F
#define VGA_CURSOR_HIGH_BIT_REG 0x0E
#define VGA_REG_SELECT_PORT     0x3D4
#define VGA_REG_DATA_PORT       0x3D5
#define CURSOR_LOC_SHIFT        8
#define CURSOR_LOC_MASK         0xFF
#define BACKSPACE_OFFSET        1
#define FC_OFFSET               1
#define ATTRIB 					0x7
#define VIDEO 0xB8000


// checkpoint 2 test defines
#define LAST_RTC_TEST_INIT -1
#define LAST_READ_FILE_INIT -2
#define READ_BY_INDEX_INIT -1

int32_t printf(int8_t *format, ...);
int32_t printf_t(int8_t *format, ...);
void putc(uint8_t c);
int32_t put_t(uint8_t* s, uint32_t size, int32_t flag);
int32_t puts(int8_t *s);
int8_t *itoa(uint32_t value, int8_t* buf, int32_t radix);
int8_t *strrev(int8_t* s);
uint32_t strlen(const int8_t* s);
void clear(void);
void clear_and_reset(void);
void clear_video_and_backing_stores();
extern void initialize_video_memory();
void set_screen_x_y(int x, int y);
int get_screen_x();
int get_screen_y();

void change_atribute(uint8_t attribute);
void shift_screen_up(void);
void change_atribute_specific(int32_t position, uint8_t attribute);
void set_cursor_location(int x, int y);
void putc_kbd(uint8_t c);
void* memset(void* s, int32_t c, uint32_t n);
void* memset_word(void* s, int32_t c, uint32_t n);
void* memset_dword(void* s, int32_t c, uint32_t n);
void* memcpy(void* dest, const void* src, uint32_t n);
void* memmove(void* dest, const void* src, uint32_t n);
int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n);
int8_t* strcpy(int8_t* dest, const int8_t*src);
int8_t* strncpy(int8_t* dest, const int8_t*src, uint32_t n);

/* Userspace address-check functions */
int32_t bad_userspace_addr(const void* addr, int32_t len);
int32_t safe_strncpy(int8_t* dest, const int8_t* src, int32_t n);

// for testing the RTC interrupt - given function, just added to header
void test_interrupts(void);

/* Port read functions */
/* Inb reads a byte and returns its value as a zero-extended 32-bit
 * unsigned int */
static inline uint32_t inb(port)
{
	uint32_t val;
	asm volatile("xorl %0, %0\n \
			inb   (%w1), %b0"
			: "=a"(val)
			: "d"(port)
			: "memory" );
	return val;
}

/* Reads two bytes from two consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them zero-extended
 * */
static inline uint32_t inw(port)
{
	uint32_t val;
	asm volatile("xorl %0, %0\n   \
			inw   (%w1), %w0"
			: "=a"(val)
			: "d"(port)
			: "memory" );
	return val;
}

/* Reads four bytes from four consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them */
static inline uint32_t inl(port)
{
	uint32_t val;
	asm volatile("inl   (%w1), %0"
			: "=a"(val)
			: "d"(port)
			: "memory" );
	return val;
}

// Forces CPU to wait for I/O to complete. See osdev.org.
// Used for PIC/other device initialization where we don't receive a response
// from the device
static inline void io_wait(void)
{
    asm volatile (
        "outb %%al, $0x80" : : "a"(0)
    );
}

/* Writes a byte to a port */
#define outb(data, port)                \
do {                                    \
	asm volatile("outb  %b1, (%w0)"     \
			:                           \
			: "d" (port), "a" (data)    \
			: "memory", "cc" );         \
} while(0)

/* Writes two bytes to two consecutive ports */
#define outw(data, port)                \
do {                                    \
	asm volatile("outw  %w1, (%w0)"     \
			:                           \
			: "d" (port), "a" (data)    \
			: "memory", "cc" );         \
} while(0)

/* Writes four bytes to four consecutive ports */
#define outl(data, port)                \
do {                                    \
	asm volatile("outl  %l1, (%w0)"     \
			:                           \
			: "d" (port), "a" (data)    \
			: "memory", "cc" );         \
} while(0)

/* Clear interrupt flag - disables interrupts on this processor */
#define cli()                           \
do {                                    \
	asm volatile("cli"                  \
			:                       \
			:                       \
			: "memory", "cc"        \
			);                      \
} while(0)

/* Save flags and then clear interrupt flag
 * Saves the EFLAGS register into the variable "flags", and then
 * disables interrupts on this processor */
#define cli_and_save(flags)             \
do {                                    \
	asm volatile("pushfl        \n      \
			popl %0         \n      \
			cli"                    \
			: "=r"(flags)           \
			:                       \
			: "memory", "cc"        \
			);                      \
} while(0)

/* Set interrupt flag - enable interrupts on this processor */
#define sti()                           \
do {                                    \
	asm volatile("sti"                  \
			:                       \
			:                       \
			: "memory", "cc"        \
			);                      \
} while(0)

/* Restore flags
 * Puts the value in "flags" into the EFLAGS register.  Most often used
 * after a cli_and_save_flags(flags) */
#define restore_flags(flags)            \
do {                                    \
	asm volatile("pushl %0      \n      \
			popfl"                  \
			:                       \
			: "r"(flags)            \
			: "memory", "cc"        \
			);                      \
} while(0)


#define get_flags(flags)             \
do {                                    \
	asm volatile("pushfl        \n      \
			popl %0"                \
			: "=r"(flags)           \
			:                       \
			: "memory", "cc"        \
			);                      \
} while(0)

#endif /* _LIB_H */
