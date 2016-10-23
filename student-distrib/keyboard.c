#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "types.h"
#include "filesystem.h"

// checkpoint 2
#define RTC_LINE_NO 8
volatile int testVal;
int rtcTest = 0;
int rtcTestNumber = 0;
int readByIndex = 0;
int first_rtc_disable = 1;
int can_print_by_name;
int can_ls;
int interrupt_seen;
const int rtcTestArray[RTC_MODES] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

volatile int allowed_to_read = 0;          // allows read to stop blocking
volatile int read_waiting = 0;

// bit 0 = shift
// bit 1 = control
// bit 2 = caps lock
uint8_t keyboard_state = 0;

uint8_t stdin[KEYBOARD_BUFF_SIZE];       // number of chars in a row is 80 ---> why do we want 128 then?
int stdin_index;                     // points to current free spot in stdin

uint8_t isOpen = 0;

//TODO: make seperate drivers for keybord and terminal

/*
 * get_char
 * DESCRIPTION: Retrieves scancode(s) from control data port
 * INPUT: none.
 * OUTPUTS: scancode(s) present in control data port
 * RETURN VALUE: none.
 * SIDE EFFECTS: Drains keyboard input
*/
uint8_t get_char()
{
    uint8_t data = inb(KEYBOARD_DATA_PORT);

    // get second byte of multi-byte sequences
    if(data == MB_SEQ_INIT) {
        data = inb(KEYBOARD_DATA_PORT);
    }

    return data;
}

/*
 * init_kbd
 * DESCRIPTION: Initializes the connected PS2 keyboard on boot-up.
 * INPUT: none.
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
void init_kbd()
{
    // set up the scancode table
    init_scancode_table();

    // init keyboard buffer attributes and fill keyboard buffer w/ null bytes
    stdin_index = 0;
    memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);

    // checkpoint 2 test
    testVal = TEST_ZERO;

    //TODO: try to change the mode of the keyboard

    // enable the interrupt on the PIC
    enable_irq(KEYBOARD_LINE_NO);
}

/*
 * open_terminal
 * DESCRIPTION: opens file to acces the stdin from kbd
 * INPUT:       const uint8_t * pathname - path to file being opened
 * OUTPUTS:  file descriptor
 * RETURN VALUE: int fd - unsigned number if success, -1 if unable to open file
 * SIDE EFFECTS: user has access to the file
*/
int32_t open_terminal(const uint8_t *pathname)
{
    isOpen = 1;
    return 0;
}

/*
 * close_terminal
 * DESCRIPTION: close file access to the stdout to the terminal
 * INPUT:   int32_t fd - file descriptor to the file being closed
 * OUTPUTS: error state
 * RETURN VALUE: int error - 0 if success and -1 if error
 * SIDE EFFECTS: uses gives up access to std out
*/
int32_t close_terminal(int32_t fd)
{
    isOpen = 0;
    return 0;
}


/*
 * read_terminal
 * DESCRIPTION: gets pointer to input buffer
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy to
 *          int32_t nbytes - number of bytes to copy to user (stops at null or this number)
 * OUTPUTS: copy stdin to user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user gets copy of the current stdin
*/
int32_t read_terminal(int32_t fd, void * buf, int32_t nbytes)
{
    int i;
    unsigned long flags;

    uint8_t * buffer = (uint8_t*) buf;

    if(nbytes < 0)
        return -1;

    //read waiting
    read_waiting = 1;

    // if we haven't seen a new line then keep waiting
    while(allowed_to_read == 0);
  
    cli_and_save(flags);
    while(stdin[i] != NULL_CHAR && i < KEYBOARD_BUFF_SIZE)
    {
        buffer[i] = stdin[i];
        i++;
    }
    
    memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
    stdin_index = 0;
    allowed_to_read = 0;
    read_waiting = 0;
    restore_flags(flags);

    return i;
}

/*
 * write_terminal
 * DESCRIPTION: does nothing
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy from
 *          int32_t nbytes - number of bytes to copy from user
 *          int32_t flag  - tells us if we are trying to redirect stdin
 * OUTPUTS: copy stdin from user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user outputs their string
*/
int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes, int32_t flag)
{
    /*
     * As design decision no bounds check will be performed on input
     * The user should know to null terminate their buffer
     */

    if(nbytes < 0)
        return -1;

    put_t((uint8_t *)buf, nbytes, flag);

    return nbytes;
}


/*
 * process_sent_scancode
 * DESCRIPTION: returns an unsigned long containing characters to display in the two most significant bits
 *                              in the 3 least significant bits returns the (potentially) updated keyboard_state
 *                              1xx control is on, x1x capslock is on, xx1 shift is on
 * INPUT: keyboard_state indicating current keyboard_state of shift, capslock, and control
 * OUTPUTS: none.
 * RETURN VALUE: none.
 * SIDE EFFECTS: Initializes the keyboard
*/
unsigned long process_sent_scancode()
{
    scancode_t mapped;
    uint8_t raw_scancode = get_char();

    /*determines map to use either controlmap, shiftmap, or normal map
    control has highest priority, and therefore goes to control map
    */

    // see if we need to update state
    switch(raw_scancode) {
        case (BACKSPACE_PRESS):
            TURN_BACKSPACE_ON(keyboard_state);
            break;
        case (BACKSPACE_RELEASE):
            TURN_BACKSPACE_OFF(keyboard_state);
            break;
        case (CAPS_LOCK_PRESS):
            TOGGLE_CAPS_LOCK(keyboard_state);
            break;
        case (CAPS_LOCK_RELEASE):
            //don't care about this
            break;
        case (SHIFT_PRESS):
            TURN_SHIFT_ON(keyboard_state);
            break;
        case (SHIFT_RELEASE):
            TURN_SHIFT_OFF(keyboard_state);
            break;
        case (CONTROL_PRESS):
            TURN_CONTROL_ON(keyboard_state);
            break;
        case (CONTROL_RELEASE):
            TURN_CONTROL_OFF(keyboard_state);
            break;
    }

    mapped = scancode_table[raw_scancode];

    if(!IS_MAKE_SC(mapped)) {
        return keyboard_state;
    }

    if(CONTROL_ON(keyboard_state)) {
        if(mapped.result == CLEAR_SCREEN_SHORTCUT) {
            clear_and_reset();
            set_cursor_location(0,0);
            printf_t("%s",stdin);
            return keyboard_state;
        }
        // checkpoint 2 tests
        switch(mapped.result) {
          	case (ASCII_ZERO):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 1;
                clear_and_reset();
                set_cursor_location(0,0);
                testVal = TEST_ZERO;
                rtcTestNumber = 0;
                rtcTest = 1;
                return keyboard_state;
			case (ASCII_ONE):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 0;
                if(can_ls)
                {
                    clear_and_reset();
                    set_cursor_location(0,0);
                }
                testVal = TEST_ONE;
                rtcTestNumber = 0;
                rtcTest = 1;
                return keyboard_state;
			case (ASCII_TWO):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 0;
                if(can_print_by_name)
                {
                    clear_and_reset();
                    set_cursor_location(0,0);
                }
                testVal = TEST_TWO;
                rtcTestNumber = 0;
                rtcTest = 1;
                return keyboard_state;
			case (ASCII_THREE):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 1;
                clear_and_reset();
                set_cursor_location(0,0);
                int num_files;
                get_dir_entries_array(&num_files);
                readByIndex++;
                testVal = TEST_THREE;
                if(readByIndex >= num_files) {
                    readByIndex = 0;
                }
                rtcTestNumber = 0;
                rtcTest = 1;
                return keyboard_state;
			case (ASCII_FOUR):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 1;
                first_rtc_disable = 1;
                clear_and_reset();
                set_cursor_location(0,0);
                enable_irq(RTC_LINE_NO);
                testVal = TEST_FOUR;
                if(rtcTestNumber >= RTC_MODES)
                    rtcTestNumber = 0;
                else
                    rtcTestNumber++;
                rtcTest = rtcTestArray[rtcTestNumber];
                return keyboard_state;
			case (ASCII_FIVE):
                memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
                stdin_index = 0;
                allowed_to_read = 0;
                interrupt_seen = 1;
                clear_and_reset();
                set_cursor_location(0,0);
                testVal = TEST_FIVE;
                rtcTestNumber = 0;
                rtcTest = 1;
                return keyboard_state;
            default:
                return keyboard_state;
		}
	} else if(testVal != TEST_ZERO)
    {
        return keyboard_state;
    } else if(BACKSPACE_ON(keyboard_state)){
        if(stdin_index > 0) {
            putc_kbd(BKSP_CHAR);
            stdin[--stdin_index] = NULL_CHAR;
        }
    } else if(mapped.result == NEW_LINE && stdin_index < KEYBOARD_BUFF_SIZE-1) {
        putc_kbd(NEW_LINE);
        stdin[stdin_index++] = NEW_LINE;
        stdin[stdin_index] = NULL_CHAR;
        if(read_waiting == 1){
            allowed_to_read = 1;
        }
        else{
            memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
            stdin_index = 0;
            allowed_to_read = 0;
        }
        return keyboard_state;
    }

    if(stdin_index >= KEYBOARD_BUFF_SIZE-NULL_NL_PADDING) {
        return keyboard_state;
    }

    if(CAPS_LOCK_ON(keyboard_state) && !SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped)) {
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL);
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            putc_kbd(mapped.result);
            stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        }
    } else if(SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped) && !CAPS_LOCK_ON(keyboard_state)) {
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL); // shifting letters is simple
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            if(non_alpha_shift_table[mapped.result] != 0) {
                putc_kbd(non_alpha_shift_table[mapped.result]);
                stdin[stdin_index++] = (non_alpha_shift_table[mapped.result]);
                if(stdin[stdin_index] != BKSP_CHAR)
                    stdin[stdin_index] = NULL_CHAR;
            } else {
                putc_kbd(mapped.result);
                stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
                if(stdin[stdin_index] != BKSP_CHAR)
                    stdin[stdin_index] = NULL_CHAR;
            }
        }
    } else {
        if(IS_PRINTABLE_SC(mapped)) {
            putc_kbd(mapped.result);
            stdin[stdin_index++] = (mapped.result);
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        }
    }

    return keyboard_state; // return current kb state
}
