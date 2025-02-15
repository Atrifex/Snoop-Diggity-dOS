/* lib.c - Some basic library functions (printf, strlen, etc.)
 * vim:ts=4 noexpandtab
 */

#include "lib.h"
#include "devices/keyboard.h"
#define NUM_COLS 80
#define NUM_ROWS 25

static int screen_x;
static int screen_y;
static char* video_mem = (char *)VIDEO;
static int curr_attribute = ATTRIB;

void set_screen_x_y(int x, int y)
{
	screen_x = x;
	screen_y = y;
	// set_cursor_location(screen_x, screen_y);
}

/*
* void get_screen_x();
*   Description: getter function for screen_x
*   Inputs: void
*   Return Value: screen_x
*	Side effects: see description
*/
int get_screen_x()
{
	return screen_x;
}

/*
* void get_screen_y();
*   Description: getter function for screen_y
*   Inputs: void
*   Return Value: screen_y
*	Side effects: see description
*/
int get_screen_y()
{
	return screen_y;
}

/*
* void clear(void);
*   Inputs: void
*   Return Value: none
*	Function: Clears video memory
*/
void
clear(void)
{
    int32_t i;
    for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = curr_attribute;
    }
}

/*
* void clear_video_and_backing_stores();
*   Description: Clears video memory and backing stores for all 3 terminals
*   Inputs: void
*   Return Value: none
*	Side effects: see description
*/
void clear_video_and_backing_stores()
{
    int32_t i;
    for(i=0; i<(NUM_ROWS*NUM_COLS)*(NUM_TERMINALS+1); i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = curr_attribute;
    }
}

/*
* void set_cursor_location(int x, int y)
*   Inputs: x and y coordinates for the cursor
*   Return Value: none
*	Side effects: Updates cursor location in VGA HW
*/
void set_cursor_location(int x, int y) {
    if(x < 0 || y < 0 || x >= NUM_COLS || y >= NUM_ROWS) {
        return;
    }

    int cursor_location = y*NUM_COLS + x;

    outb(VGA_CURSOR_LOW_BIT_REG, VGA_REG_SELECT_PORT);
    outb((uint8_t) (cursor_location & CURSOR_LOC_MASK), VGA_REG_DATA_PORT);

    outb(VGA_CURSOR_HIGH_BIT_REG, VGA_REG_SELECT_PORT);
    outb((uint8_t) ( (cursor_location >> CURSOR_LOC_SHIFT ) & CURSOR_LOC_MASK ), VGA_REG_DATA_PORT);
}

/*
* void clear_and_reset(void);
*   Inputs: void
*   Return Value: none
*	Function: Clears video memory and resets the values of screen_x and screen_y to 0.
* 			  This allows printing at the start of the screen.
*/
void clear_and_reset(void)
{
	clear();
	screen_x = 0;
	screen_y = 0;
}

/*
* void initialize_video_memory();
* Inputs: NONE
* Return value: NONE
* Function: Initializes video memory and sets global cursor pos to 0
*/
void initialize_video_memory()
{
	clear_video_and_backing_stores();
	screen_x = 0;
	screen_y = 0;
}

/*
* void shift_screen_up(void);
*   Inputs: void
*   Return Value: none
*	Function: shifts everything in video memory up so that we can account for a new line
*/
void shift_screen_up(void)
{
	int32_t i;
    // shifts up NUM_ROWS - 1 rows
    for(i=0; i<(NUM_ROWS-1)*NUM_COLS; i++) {
        *(uint8_t *)(video_mem + (i << 1)) = *(uint8_t *)(video_mem + ((i + NUM_COLS) << 1));
        *(uint8_t *)(video_mem + (i << 1) + 1) = *(uint8_t *)(video_mem + ((i + NUM_COLS) << 1) + 1);
    }
    // clears last row to be used
    for(i=0; i<NUM_COLS; i++) {
        *(uint8_t *)(video_mem + ((i + (NUM_ROWS-1)*NUM_COLS) << 1)) = ' ';
        *(uint8_t *)(video_mem + ((i + (NUM_ROWS-1)*NUM_COLS) << 1) + 1) = curr_attribute;
    }

    screen_y--;
}

/*
* void putc_kbd(uint8_t c);
*   Description: ouputs char to the screen, has special cases for new line and back space
*   Inputs: uint_8* c = character to print
*   Return Value: void
*	Side Effect: makes a character appear on the screen
*/
void
putc_kbd(uint8_t c)
{
    if(c == '\n' || c == '\r') {
        // if new line, make the screen point to the next line
        screen_y++;
        screen_x=0;
    } else if(c == BKSP_CHAR){
        // if BKSP_CHAR, then decrement appropriate values
    	screen_x--;

        // if screen_x becomes < 0, then make it wrap around decrement y
    	if(screen_x < 0)
    	{
    		screen_x = NUM_COLS -1;
    		screen_y--;
            // if screen_y becomes < 0, then reset it to 0
    		if(screen_y < 0)
    			screen_y = 0;
    	}
        // place EMPTY_SPACE char at correct position in memory
    	*(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1)) = EMPTY_SPACE;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1) + 1) = curr_attribute;
    } else {
        // place given character on screen with its specific attribute
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1)) = c;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1) + 1) = curr_attribute;

		/*
		 * I am very angry because in their code the had the assignment of screen_y after screen_x %= NUM_COLS
		 * This basically made it have no effect and wasted hours of my time.
		 */
        screen_x++;
        screen_y = (screen_y + (screen_x / NUM_COLS)); // % NUM_ROWS;
        screen_x %= NUM_COLS;
    }

    // if screen_y becomes >= NUM_ROWS during this process, then shift the screen up
    if(screen_y >= NUM_ROWS) {
			shift_screen_up();
	}

    // set cursor to appropriate location
    set_cursor_location(screen_x, screen_y);
}



/*
* void putc_screen(uint8_t c);
*   Description: puts character onto screen
*   Inputs: uint_8* c = character to print
*   Return Value: void
*	Side Effect: makes a character appear on the screen
*/
void
putc_screen(uint8_t c)
{
    if(c == '\n' || c == '\r') {
        // if new line, make the screen point to the next line
        screen_y++;
        screen_x=0;
    } else if(c == BKSP_CHAR){
        // if BKSP_CHAR, then decrement appropriate values
    	screen_x--;

        // if screen_x becomes < 0, then make it wrap around decrement y
    	if(screen_x < 0)
    	{
    		screen_x = NUM_COLS -1;
    		screen_y--;
            // if screen_y becomes < 0, then reset it to 0
    		if(screen_y < 0)
    			screen_y = 0;
    	}
        // place EMPTY_SPACE char at correct position in memory
    	*(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1)) = EMPTY_SPACE;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1) + 1) = curr_attribute;
    } else {
        // place given character on screen with its specific attribute
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1)) = c;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1) + 1) = curr_attribute;

		/*
		 * I am very angry because in their code the had the assignment of screen_y after screen_x %= NUM_COLS
		 * This basically made it have no effect and wasted hours of my time.
		 */
        screen_x++;
        screen_y = (screen_y + (screen_x / NUM_COLS)); // % NUM_ROWS;
        screen_x %= NUM_COLS;
    }

    // if screen_y becomes >= NUM_ROWS during this process, then shift the screen up
    if(screen_y >= NUM_ROWS) {
			shift_screen_up();
	}

    // set cursor to appropriate location
    // set_cursor_location(screen_x, screen_y);
}


/*
* int32_t put_t(int8_t* s);
*   Inputs: int_8* s = pointer to a string of characters, uint32_t size, int32_t flag
*   Return Value: Number of bytes written
*	Function: Output a string to the console
*/
int32_t
put_t(uint8_t* s, uint32_t size, int32_t flag)
{
    register int32_t index = 0;
    while(s[index] != NULL_CHAR && index < size) {
    	putc_screen(s[index]);
        index++;
    }
    return index;
}

/*
* void change_atribute(void);
*   Inputs: void
*   Return Value: none
*	Function: change attribute byte for all of video memory
*/
void change_atribute(uint8_t attribute)
{
	int32_t i;
    for(i=0; i<NUM_ROWS*NUM_COLS; i++) {
        // assign provided attribute value to all of video memory
        *(uint8_t *)(video_mem + (i << 1) + 1) = attribute;
    }
    curr_attribute = attribute;
}

/*
* void change_atribute_specific(void);
*   Inputs: void
*   Return Value: none
*	Function: change attribute at a specific location in video memory
*/
void change_atribute_specific(int32_t position, uint8_t attribute)
{
    // assign attribute to given location in video memory
    *(uint8_t *)(video_mem + (position << 1) + 1) = attribute;
}


/* Standard printf().
 * Only supports the following format strings:
 * %%  - print a literal '%' character
 * %x  - print a number in hexadecimal
 * %u  - print a number as an unsigned integer
 * %d  - print a number as a signed integer
 * %c  - print a character
 * %s  - print a string
 * %#x - print a number in 32-bit aligned hexadecimal, i.e.
 *       print 8 hexadecimal digits, zero-padded on the left.
 *       For example, the hex number "E" would be printed as
 *       "0000000E".
 *       Note: This is slightly different than the libc specification
 *       for the "#" modifier (this implementation doesn't add a "0x" at
 *       the beginning), but I think it's more flexible this way.
 *       Also note: %x is the only conversion specifier that can use
 *       the "#" modifier to alter output.
 * */
int32_t
printf(int8_t *format, ...)
{
	/* Pointer to the format string */
	int8_t* buf = format;

	/* Stack pointer for the other parameters */
	int32_t* esp = (void *)&format;
	esp++;

	while(*buf != '\0') {
		switch(*buf) {
			case '%':
				{
					int32_t alternate = 0;
					buf++;

format_char_switch:
					/* Conversion specifiers */
					switch(*buf) {
						/* Print a literal '%' character */
						case '%':
							putc('%');
							break;

						/* Use alternate formatting */
						case '#':
							alternate = 1;
							buf++;
							/* Yes, I know gotos are bad.  This is the
							 * most elegant and general way to do this,
							 * IMHO. */
							goto format_char_switch;

						/* Print a number in hexadecimal form */
						case 'x':
							{
								int8_t conv_buf[64];
								if(alternate == 0) {
									itoa(*((uint32_t *)esp), conv_buf, 16);
									puts(conv_buf);
								} else {
									int32_t starting_index;
									int32_t i;
									itoa(*((uint32_t *)esp), &conv_buf[8], 16);
									i = starting_index = strlen(&conv_buf[8]);
									while(i < 8) {
										conv_buf[i] = '0';
										i++;
									}
									puts(&conv_buf[starting_index]);
								}
								esp++;
							}
							break;

						/* Print a number in unsigned int form */
						case 'u':
							{
								int8_t conv_buf[36];
								itoa(*((uint32_t *)esp), conv_buf, 10);
								puts(conv_buf);
								esp++;
							}
							break;

						/* Print a number in signed int form */
						case 'd':
							{
								int8_t conv_buf[36];
								int32_t value = *((int32_t *)esp);
								if(value < 0) {
									conv_buf[0] = '-';
									itoa(-value, &conv_buf[1], 10);
								} else {
									itoa(value, conv_buf, 10);
								}
								puts(conv_buf);
								esp++;
							}
							break;

						/* Print a single character */
						case 'c':
							putc( (uint8_t) *((int32_t *)esp) );
							esp++;
							break;

						/* Print a NULL-terminated string */
						case 's':
							puts( *((int8_t **)esp) );
							esp++;
							break;

						default:
							break;
					}

				}
				break;

			default:
				putc(*buf);
				break;
		}
		buf++;
	}

	return (buf - format);
}

/* Standard printf_t() - but writes to our terminal driver
 * Only supports the following format strings:
 * %%  - print a literal '%' character
 * %x  - print a number in hexadecimal
 * %u  - print a number as an unsigned integer
 * %d  - print a number as a signed integer
 * %c  - print a character
 * %s  - print a string
 * %#x - print a number in 32-bit aligned hexadecimal, i.e.
 *       print 8 hexadecimal digits, zero-padded on the left.
 *       For example, the hex number "E" would be printed as
 *       "0000000E".
 *       Note: This is slightly different than the libc specification
 *       for the "#" modifier (this implementation doesn't add a "0x" at
 *       the beginning), but I think it's more flexible this way.
 *       Also note: %x is the only conversion specifier that can use
 *       the "#" modifier to alter output.
 * */
int32_t printf_t(int8_t *format, ...)
{
	/* Pointer to the format string */
	int8_t* buf = format;

	/* Stack pointer for the other parameters */
	int32_t* esp = (void *)&format;
	esp++;

	while(*buf != '\0') {
		switch(*buf) {
			case '%':
				{
					int32_t alternate = 0;
					buf++;

format_char_switch:
					/* Conversion specifiers */
					switch(*buf) {
						/* Print a literal '%' character */
						case '%':
							write_terminal(STDOUT, "%", 1);
							break;

						/* Use alternate formatting */
						case '#':
							alternate = 1;
							buf++;
							/* Yes, I know gotos are bad.  This is the
							 * most elegant and general way to do this,
							 * IMHO. */
							goto format_char_switch;

						/* Print a number in hexadecimal form */
						case 'x':
							{
								int8_t conv_buf[64];
								if(alternate == 0) {
									itoa(*((uint32_t *)esp), conv_buf, 16);
									write_terminal(STDOUT, conv_buf, sizeof(conv_buf));
									//puts(conv_buf);
								} else {
									int32_t starting_index;
									int32_t i;
									itoa(*((uint32_t *)esp), &conv_buf[8], 16);
									i = starting_index = strlen(&conv_buf[8]);
									while(i < 8) {
										conv_buf[i] = '0';
										i++;
									}
									write_terminal(STDOUT, &conv_buf[starting_index], sizeof(conv_buf) - starting_index);

									// puts(&conv_buf[starting_index]);
								}
								esp++;
							}
							break;

						/* Print a number in unsigned int form */
						case 'u':
							{
								int8_t conv_buf[36];
								itoa(*((uint32_t *)esp), conv_buf, 10);
								write_terminal(STDOUT, conv_buf, sizeof(conv_buf));
								esp++;
							}
							break;

						/* Print a number in signed int form */
						case 'd':
							{
								int8_t conv_buf[36];
								int32_t value = *((int32_t *)esp);
								if(value < 0) {
									conv_buf[0] = '-';
									itoa(-value, &conv_buf[1], 10);
								} else {
									itoa(value, conv_buf, 10);
								}
								write_terminal(STDOUT, conv_buf, sizeof(conv_buf));
								esp++;
							}
							break;

						/* Print a single character */
						case 'c':
							write_terminal(STDOUT, ((uint8_t *)esp), 1);
							esp++;
							break;

						/* Print a NULL-terminated string */
						case 's':
							// puts( *((int8_t **)esp) );
							write_terminal(STDOUT, *((int8_t **)esp), strlen(*((int8_t **)esp)));
							esp++;
							break;

						default:
							break;
					}

				}
				break;

			default:
				// c = *buf;
				write_terminal(STDOUT, buf, 1);
				break;
		}
		buf++;
	}

	return (buf - format);
}

/*
* int32_t puts(int8_t* s);
*   Inputs: int_8* s = pointer to a string of characters
*   Return Value: Number of bytes written
*	Function: Output a string to the console
*/

int32_t
puts(int8_t* s)
{
	register int32_t index = 0;
	while(s[index] != '\0') {
		putc(s[index]);
		index++;
	}
	return index;
}

/*
* void putc(uint8_t c);
*   Inputs: uint_8* c = character to print
*   Return Value: void
*	Function: Output a character to the console
*/

void
putc(uint8_t c)
{
    if(c == '\n' || c == '\r') {
        screen_y++;
        screen_x=0;
    } else {
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1)) = c;
        *(uint8_t *)(video_mem + ((NUM_COLS*screen_y + screen_x) << 1) + 1) = curr_attribute;

		/*
		 * I am very angry because in their code the had the assignment of screen_y after screen_x %= NUM_COLS
		 * This basically made it have no effect and wasted hours of my time.
		 */
        screen_x++;
        screen_y = (screen_y + (screen_x / NUM_COLS)); // % NUM_ROWS;
        screen_x %= NUM_COLS;
		// TODO: need to shift screen up when out of bounds
		if(screen_y >= NUM_ROWS) {
			shift_screen_up();
		}
    }
}

/*
* int8_t* itoa(uint32_t value, int8_t* buf, int32_t radix);
*   Inputs: uint32_t value = number to convert
*			int8_t* buf = allocated buffer to place string in
*			int32_t radix = base system. hex, oct, dec, etc.
*   Return Value: number of bytes written
*	Function: Convert a number to its ASCII representation, with base "radix"
*/

int8_t*
itoa(uint32_t value, int8_t* buf, int32_t radix)
{
	static int8_t lookup[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	int8_t *newbuf = buf;
	int32_t i;
	uint32_t newval = value;

	/* Special case for zero */
	if(value == 0) {
		buf[0]='0';
		buf[1]='\0';
		return buf;
	}

	/* Go through the number one place value at a time, and add the
	 * correct digit to "newbuf".  We actually add characters to the
	 * ASCII string from lowest place value to highest, which is the
	 * opposite of how the number should be printed.  We'll reverse the
	 * characters later. */
	while(newval > 0) {
		i = newval % radix;
		*newbuf = lookup[i];
		newbuf++;
		newval /= radix;
	}

	/* Add a terminating NULL */
	*newbuf = '\0';

	/* Reverse the string and return */
	return strrev(buf);
}

/*
* int8_t* strrev(int8_t* s);
*   Inputs: int8_t* s = string to reverse
*   Return Value: reversed string
*	Function: reverses a string s
*/

int8_t*
strrev(int8_t* s)
{
	register int8_t tmp;
	register int32_t beg=0;
	register int32_t end=strlen(s) - 1;

	while(beg < end) {
		tmp = s[end];
		s[end] = s[beg];
		s[beg] = tmp;
		beg++;
		end--;
	}

	return s;
}

/*
* uint32_t strlen(const int8_t* s);
*   Inputs: const int8_t* s = string to take length of
*   Return Value: length of string s
*	Function: return length of string s
*/

uint32_t
strlen(const int8_t* s)
{
	register uint32_t len = 0;
	while(s[len] != '\0')
		len++;

	return len;
}

/*
* void* memset(void* s, int32_t c, uint32_t n);
*   Inputs: void* s = pointer to memory
*			int32_t c = value to set memory to
*			uint32_t n = number of bytes to set
*   Return Value: new string
*	Function: set n consecutive bytes of pointer s to value c
*/

void*
memset(void* s, int32_t c, uint32_t n)
{
	c &= 0xFF;
	asm volatile("                  \n\
			.memset_top:            \n\
			testl   %%ecx, %%ecx    \n\
			jz      .memset_done    \n\
			testl   $0x3, %%edi     \n\
			jz      .memset_aligned \n\
			movb    %%al, (%%edi)   \n\
			addl    $1, %%edi       \n\
			subl    $1, %%ecx       \n\
			jmp     .memset_top     \n\
			.memset_aligned:        \n\
			movw    %%ds, %%dx      \n\
			movw    %%dx, %%es      \n\
			movl    %%ecx, %%edx    \n\
			shrl    $2, %%ecx       \n\
			andl    $0x3, %%edx     \n\
			cld                     \n\
			rep     stosl           \n\
			.memset_bottom:         \n\
			testl   %%edx, %%edx    \n\
			jz      .memset_done    \n\
			movb    %%al, (%%edi)   \n\
			addl    $1, %%edi       \n\
			subl    $1, %%edx       \n\
			jmp     .memset_bottom  \n\
			.memset_done:           \n\
			"
			:
			: "a"(c << 24 | c << 16 | c << 8 | c), "D"(s), "c"(n)
			: "edx", "memory", "cc"
			);

	return s;
}

/*
* void* memset_word(void* s, int32_t c, uint32_t n);
*   Inputs: void* s = pointer to memory
*			int32_t c = value to set memory to
*			uint32_t n = number of bytes to set
*   Return Value: new string
*	Function: set lower 16 bits of n consecutive memory locations of pointer s to value c
*/

/* Optimized memset_word */
void*
memset_word(void* s, int32_t c, uint32_t n)
{
	asm volatile("                  \n\
			movw    %%ds, %%dx      \n\
			movw    %%dx, %%es      \n\
			cld                     \n\
			rep     stosw           \n\
			"
			:
			: "a"(c), "D"(s), "c"(n)
			: "edx", "memory", "cc"
			);

	return s;
}

/*
* void* memset_dword(void* s, int32_t c, uint32_t n);
*   Inputs: void* s = pointer to memory
*			int32_t c = value to set memory to
*			uint32_t n = number of bytes to set
*   Return Value: new string
*	Function: set n consecutive memory locations of pointer s to value c
*/

void*
memset_dword(void* s, int32_t c, uint32_t n)
{
	asm volatile("                  \n\
			movw    %%ds, %%dx      \n\
			movw    %%dx, %%es      \n\
			cld                     \n\
			rep     stosl           \n\
			"
			:
			: "a"(c), "D"(s), "c"(n)
			: "edx", "memory", "cc"
			);

	return s;
}


/*
* void* memcpy(void* dest, const void* src, uint32_t n);
*   Inputs: void* dest = destination of copy
*			const void* src = source of copy
*			uint32_t n = number of byets to copy
*   Return Value: pointer to dest
*	Function: copy n bytes of src to dest
*/

void*
memcpy(void* dest, const void* src, uint32_t n)
{
	asm volatile("                  \n\
			.memcpy_top:            \n\
			testl   %%ecx, %%ecx    \n\
			jz      .memcpy_done    \n\
			testl   $0x3, %%edi     \n\
			jz      .memcpy_aligned \n\
			movb    (%%esi), %%al   \n\
			movb    %%al, (%%edi)   \n\
			addl    $1, %%edi       \n\
			addl    $1, %%esi       \n\
			subl    $1, %%ecx       \n\
			jmp     .memcpy_top     \n\
			.memcpy_aligned:        \n\
			movw    %%ds, %%dx      \n\
			movw    %%dx, %%es      \n\
			movl    %%ecx, %%edx    \n\
			shrl    $2, %%ecx       \n\
			andl    $0x3, %%edx     \n\
			cld                     \n\
			rep     movsl           \n\
			.memcpy_bottom:         \n\
			testl   %%edx, %%edx    \n\
			jz      .memcpy_done    \n\
			movb    (%%esi), %%al   \n\
			movb    %%al, (%%edi)   \n\
			addl    $1, %%edi       \n\
			addl    $1, %%esi       \n\
			subl    $1, %%edx       \n\
			jmp     .memcpy_bottom  \n\
			.memcpy_done:           \n\
			"
			:
			: "S"(src), "D"(dest), "c"(n)
			: "eax", "edx", "memory", "cc"
			);

	return dest;
}

/*
* void* memmove(void* dest, const void* src, uint32_t n);
*   Inputs: void* dest = destination of move
*			const void* src = source of move
*			uint32_t n = number of byets to move
*   Return Value: pointer to dest
*	Function: move n bytes of src to dest
*/

/* Optimized memmove (used for overlapping memory areas) */
void*
memmove(void* dest, const void* src, uint32_t n)
{
	asm volatile("                  \n\
			movw    %%ds, %%dx      \n\
			movw    %%dx, %%es      \n\
			cld                     \n\
			cmp     %%edi, %%esi    \n\
			jae     .memmove_go     \n\
			leal    -1(%%esi, %%ecx), %%esi    \n\
			leal    -1(%%edi, %%ecx), %%edi    \n\
			std                     \n\
			.memmove_go:            \n\
			rep     movsb           \n\
			"
			:
			: "D"(dest), "S"(src), "c"(n)
			: "edx", "memory", "cc"
			);

	return dest;
}

/*
* int32_t strncmp(const int8_t* s1, const int8_t* s2, uint32_t n)
*   Inputs: const int8_t* s1 = first string to compare
*			const int8_t* s2 = second string to compare
*			uint32_t n = number of bytes to compare
*	Return Value: A zero value indicates that the characters compared
*					in both strings form the same string.
*				A value greater than zero indicates that the first
*					character that does not match has a greater value
*					in str1 than in str2; And a value less than zero
*					indicates the opposite.
*	Function: compares string 1 and string 2 for equality
*/

int32_t
strncmp(const int8_t* s1, const int8_t* s2, uint32_t n)
{
	int32_t i;
	for(i=0; i<n; i++) {
		if( (s1[i] != s2[i]) ||
				(s1[i] == '\0') /* || s2[i] == '\0' */ ) {

			/* The s2[i] == '\0' is unnecessary because of the short-circuit
			 * semantics of 'if' expressions in C.  If the first expression
			 * (s1[i] != s2[i]) evaluates to false, that is, if s1[i] ==
			 * s2[i], then we only need to test either s1[i] or s2[i] for
			 * '\0', since we know they are equal. */

			return s1[i] - s2[i];
		}
	}
	return 0;
}

/*
* int8_t* strcpy(int8_t* dest, const int8_t* src)
*   Inputs: int8_t* dest = destination string of copy
*			const int8_t* src = source string of copy
*   Return Value: pointer to dest
*	Function: copy the source string into the destination string
*/

int8_t*
strcpy(int8_t* dest, const int8_t* src)
{
	int32_t i=0;
	while(src[i] != '\0') {
		dest[i] = src[i];
		i++;
	}

	dest[i] = '\0';
	return dest;
}

/*
* int8_t* strcpy(int8_t* dest, const int8_t* src, uint32_t n)
*   Inputs: int8_t* dest = destination string of copy
*			const int8_t* src = source string of copy
*			uint32_t n = number of bytes to copy
*   Return Value: pointer to dest
*	Function: copy n bytes of the source string into the destination string
*/

int8_t*
strncpy(int8_t* dest, const int8_t* src, uint32_t n)
{
	int32_t i=0;
	while(src[i] != '\0' && i < n) {
		dest[i] = src[i];
		i++;
	}

	while(i < n) {
		dest[i] = '\0';
		i++;
	}

	return dest;
}

/*
* void test_interrupts(void)
*   Inputs: void
*   Return Value: void
*	Function: increments video memory. To be used to test rtc
*/

void
test_interrupts(void)
{
	int32_t i;
	for (i=0; i < NUM_ROWS*NUM_COLS; i++) {
		video_mem[i<<1]++;
	}
}
