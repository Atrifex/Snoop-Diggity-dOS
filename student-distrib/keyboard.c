#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "types.h"

// bit 0 = shift
// bit 1 = control
// bit 2 = caps lock
uint8_t keyboard_state = 0;

uint8_t stdin[KEYBOARD_BUFF_SIZE];       // number of chars in a row is 80 ---> why do we want 128 then?
uint8_t stdin_index;                     // points to current free spot in stdin


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

    uint8_t * buffer = (uint8_t*) buf;

    if(nbytes > KEYBOARD_BUFF_SIZE)
        return -1;
    else if(nbytes < 0)
        return -1;

    for(i = 0; i < nbytes; i++)
    {
        buffer[i] = stdin[i];
        // clear input if you see a new line---> however allow user to keep reading reset of buffer
        if(stdin[i] == NEW_LINE)
        {
            stdin_index = 0;
            memset(stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
            buffer[i+1] = NULL_CHAR;
            break;
        }
        if(stdin[i] == NULL_CHAR) {
            break;
        }
    }

    return i;
}

/*
 * write_terminal
 * DESCRIPTION: does nothing
 * INPUT:   int32_t fd - file descriptor to open fil 
 *          void * buf - pointer to buf to copy from
 *          int32_t nbytes - number of bytes to copy from user
 * OUTPUTS: copy stdin from user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied  
 *                            error: -1
 * SIDE EFFECTS: user outputs their string
*/
int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes)
{
    /*
     * As design decision no bounds check will be performed on input 
     * The user should know to null terminate their buffer
     */

    if(nbytes < 0)
        return -1;

    put_t((uint8_t *)buf);

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

    // 
    if(CONTROL_ON(keyboard_state)) {
        if(mapped.result == CLEAR_SCREEN_SHORTCUT) {
            clear_and_reset();
            return keyboard_state;
        }
    } else if(BACKSPACE_ON(keyboard_state)){
        if(stdin_index > 0) {
            stdin[--stdin_index] = BKSP_CHAR;
        }
    } else if(mapped.result == NEW_LINE && stdin_index < KEYBOARD_BUFF_SIZE-1) {
        stdin[stdin_index++] = NEW_LINE;
        stdin[stdin_index] = NULL_CHAR;
    } 
    
    if(stdin_index >= KEYBOARD_BUFF_SIZE-NULL_NL_PADDING) {
        return keyboard_state;
    }
    
    if(CAPS_LOCK_ON(keyboard_state) && !SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped)) {
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL);
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        }
    } else if(SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped) && !CAPS_LOCK_ON(keyboard_state)) {
            stdin[stdin_index++] = (mapped.result - ASCII_SHIFT_VAL); // shifting letters is simple
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            if(non_alpha_shift_table[mapped.result] != 0) {
                stdin[stdin_index++] = (non_alpha_shift_table[mapped.result]);
                if(stdin[stdin_index] != BKSP_CHAR)
                    stdin[stdin_index] = NULL_CHAR;
            } else {
                stdin[stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
                if(stdin[stdin_index] != BKSP_CHAR)
                    stdin[stdin_index] = NULL_CHAR;
            }
        }
    } else {
        if(IS_PRINTABLE_SC(mapped)) {
            stdin[stdin_index++] = (mapped.result);
            if(stdin[stdin_index] != BKSP_CHAR)
                stdin[stdin_index] = NULL_CHAR;
        }
    }
    
    return keyboard_state; // return current kb state
}
