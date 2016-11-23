#include "keyboard.h"
#include "lib.h"
#include "i8259.h"
#include "types.h"
#include "filesystem.h"

// specifies the color on the screen
int curr_attribute = ATTRIB;
int curr_back_attribute = 0;

// holds state of "command" keys
uint8_t keyboard_state = 0;

uint8_t terminals_launched = 1;      // the first terminal is launched by default
uint8_t terminal_state = 0;
terminal_t terminals[NUM_TERMINALS];

void change_terminal_state(int from, int to);

/*
 * get_terminal_state
 * DESCRIPTION: returns the currently active terminal number
 * INPUT: none
 * OUTPUTS: none
 * RETURN VALUE: currently active terminal number
 * SIDE EFFECTS: see DESCRIPTION
*/
uint8_t get_terminal_state()
{
    return terminal_state;
}

/*
 * get_launched_terminals
 * DESCRIPTION: returns the mask that shows which termainals have been launched
 * INPUT: none
 * OUTPUTS: none
 * RETURN VALUE: currently active terminal launched
 * SIDE EFFECTS: see DESCRIPTION
*/
uint8_t get_launched_terminals()
{
    return terminals_launched;
}

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
    // get first byte of data
    uint8_t data = inb(KEYBOARD_DATA_PORT);

    // get second byte of multi-byte sequences
    if(data == MB_SEQ_INIT) {
        io_wait();
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

    // initiallizes the memeber values of the terminal
    int i;
    for(i = 0; i < NUM_TERMINALS; i++){
        terminals[i].screen_x = 0;
        terminals[i].screen_y = 0;
        memset(terminals[i].stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
        terminals[i].stdin_index = 0;
        terminals[i].allowed_to_read = 0;
        terminals[i].read_waiting = 0;
    }

    // enable the interrupt on the PIC
    enable_irq(KEYBOARD_LINE_NO);
}

/*
 * open_terminal
 * DESCRIPTION: always open
 * INPUT:       const uint8_t * pathname - path to file being opened
 * OUTPUTS:  file descriptor
 * RETURN VALUE: int fd - unsigned number if success, -1 if unable to open file
 * SIDE EFFECTS: user has access to the file
*/
int32_t open_terminal(const uint8_t *pathname)
{
    return 0;
}

/*
 * close_terminal
 * DESCRIPTION: always open
 * INPUT:   int32_t fd - file descriptor to the file being closed
 * OUTPUTS: error state
 * RETURN VALUE: int error - 0 if success and -1 if error
 * SIDE EFFECTS: uses gives up access to std out
*/
int32_t close_terminal(int32_t fd)
{
    return 0;
}


/*
 * read_terminal
 * DESCRIPTION: gets pointer to input buffer
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy to
 *          int32_t nbytes - number of bytes to copy to user (stops at null or this number)
 * OUTPUTS: copy terminals[terminal_state].stdin to user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user gets copy of the current terminals[terminal_state].stdin
*/
int32_t read_terminal(int32_t fd, void * buf, int32_t nbytes)
{
    // error check
    if(fd != STDIN){
        return ERROR;
    }

    int i = 0;
    unsigned long flags;

    uint8_t terminal_state_local = terminal_state;

    // fixes type confussion
    uint8_t * buffer = (uint8_t*) buf;

    // checks for error condition
    if(nbytes < 0)
        return FAILURE;

    // tells interrupt that there is a read waiting
    terminals[terminal_state_local].read_waiting = 1;

    // if we haven't seen a new line then not allowed to read
    while(terminals[terminal_state_local].allowed_to_read == 0);

    // critical section
    cli_and_save(flags);

    // copy all information uptil the null char char
    while(terminals[terminal_state_local].stdin[i] != NULL_CHAR && i < KEYBOARD_BUFF_SIZE)
    {
        buffer[i] = terminals[terminal_state_local].stdin[i];
        i++;
    }

    // flush buffer and set terminals[terminal_state].stdin_index to 0
    memset(terminals[terminal_state_local].stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
    terminals[terminal_state_local].stdin_index = 0;

    // disallow reading
    terminals[terminal_state_local].allowed_to_read = 0;
    terminals[terminal_state_local].read_waiting = 0;
    restore_flags(flags);

    // return number of bytes read
    return i;
}

/*
 * write_terminal
 * DESCRIPTION: does nothing
 * INPUT:   int32_t fd - file descriptor to open fil
 *          void * buf - pointer to buf to copy from
 *          int32_t nbytes - number of bytes to copy from user
 *          int32_t flag  - tells us if we are trying to redirect terminals[terminal_state].stdin
 * OUTPUTS: copy terminals[terminal_state].stdin from user buf
 * RETURN VALUE: int copied - succes: the number of bytes copied
 *                            error: -1
 * SIDE EFFECTS: user outputs their string
*/
int32_t write_terminal(int32_t fd, const void *buf, int32_t nbytes)
{
    // error checks
    if(fd != STDOUT){
        return ERROR;
    }
    if(nbytes < 0){
        return FAILURE;
    }

    // print buf to the screen
    put_t((uint8_t *)buf, nbytes, 1);

    // return number of bytes read
    return nbytes;
}

/*
 * void change_terminal_state(int from, int to)
 * DESCRIPTION: Perform page table and video memory switches corresponding to a terminal switch.
 * INPUTS: int from - terminal we are switching from, to - terminal we are switching to
 * OUTPUTS: NONE
 * SIDE EFFECTS: updates terminal_state
*/
void change_terminal_state(int from, int to)
{
    int i;
    pcb_t* cur_pcb;

    // copy memory from video memory    (dest, src, num bytes)
    memcpy((void*)(VIDEO+((from+1)*LITERAL_4KB)),(void*)(VIDEO),LITERAL_4KB);
    // memcpy((void*)(VIDEO),(void*)(VIDEO+(1*LITERAL_4KB)),LITERAL_4KB);
    // memcpy((void*)(VIDEO),(void*)(VIDEO+(2*LITERAL_4KB)),LITERAL_4KB);
    // memcpy((void*)(VIDEO),(void*)(VIDEO+(3*LITERAL_4KB)),LITERAL_4KB);
    memcpy((void*)(VIDEO),(void*)(VIDEO+((to+1)*LITERAL_4KB)),LITERAL_4KB);

    // set screen locations
    terminals[from].screen_x = get_screen_x();
    terminals[from].screen_y = get_screen_y();
    set_screen_x_y(terminals[to].screen_x, terminals[to].screen_y);

    // page table entry for VIDEO in terminal state processes to pt to backing store
    // change page table entry for VIDEO in t1 processes to point to video memory
    for(i = 0; i < MAX_NUM_PROCS; i++) {
        if(!is_pid_used(i)) continue;
        // read the PCB for this PID
        cur_pcb = (pcb_t*)(KERNEL_STACK_START - (i+1)*LITERAL_8KB);
        if(cur_pcb->owned_by_terminal == from) {
            // write to a backing store
            change_table_mapping(get_base_page_table_for_pid(i), VIDEO, VIDEO+((from+1)*LITERAL_4KB));
        }

        if(cur_pcb->owned_by_terminal == to) {
            // this process should now actually write to video memory
            change_table_mapping(get_base_page_table_for_pid(i), VIDEO, VIDEO);
        }
    }

    vm_flush_page(VIDEO);

    terminal_state = to;
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

    // tss_t* tss_base = (tss_t*)&tss;
    // pcb_t* pcb_curr = (pcb_t*)((tss_base->esp0-1) & MASK_8KB_ALIGNED);
    // int pid = pcb_curr->pid;

    // get scancode
    uint8_t raw_scancode = get_char();

    if(raw_scancode == DELETE_SCAN_CODE)
        return keyboard_state;

    // see if we need to update keyboard_state
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
        case (R_SHIFT_PRESS):
            TURN_R_SHIFT_ON(keyboard_state);
            break;
        case (R_SHIFT_RELEASE):
            TURN_R_SHIFT_OFF(keyboard_state);
            break;
        case (CONTROL_PRESS):
            TURN_CONTROL_ON(keyboard_state);
            break;
        case (CONTROL_RELEASE):
            TURN_CONTROL_OFF(keyboard_state);
            break;
        case (ALT_PRESS):
            TURN_ALT_ON(keyboard_state);
            break;
        case (ALT_RELEASE):
            TURN_ALT_OFF(keyboard_state);
            break;
    }

    // get mapped value of scancode
    mapped = scancode_table[raw_scancode];

    // if not a make scancode then return
    if(!IS_MAKE_SC(mapped)) {
        return keyboard_state;
    }

    set_new_page_directory(get_page_directory_for_pid(terminals[terminal_state].pid));

    // switching terminals
    if(ALT_ON(keyboard_state)){
        cli();
        switch(mapped.result) {
            case (ASCII_ONE):
                if(terminal_state != STATE_ONE){
                    // change the configuration of video memory
                    change_terminal_state(terminal_state, STATE_ONE);
                }
                break;
            case (ASCII_TWO):
                if(terminal_state != STATE_TWO){
                    // change the configuration of video memory
                    change_terminal_state(terminal_state, STATE_TWO);
                    // execute the shell corresponding to the terminal
                    if(!(terminals_launched & TERMINAL_TWO_MASK)){
                        save_process_context((uint32_t)&&SWITCH_TERMINAL_CONTEXT, get_esp() + ACCOUNT_FOR_RET_ADDR, get_ebp());
                        terminals_launched |= TERMINAL_TWO_MASK;
                        send_eoi(KEYBOARD_LINE_NO);
                        internal_execute((uint8_t*) "shell", FIRST_TERM_SHELL);
                    }
                }
                break;
            case (ASCII_THREE):
                if(terminal_state != STATE_THREE){
                    // change the configuration of video memory
                    change_terminal_state(terminal_state, STATE_THREE);
                    // execute the shell corresponding to the terminal
                    if(!(terminals_launched & TERMINAL_THREE_MASK)){
                        save_process_context((uint32_t)&&SWITCH_TERMINAL_CONTEXT, get_esp() + ACCOUNT_FOR_RET_ADDR, get_ebp());
                        terminals_launched |= TERMINAL_THREE_MASK;
                        send_eoi(KEYBOARD_LINE_NO);
                        internal_execute((uint8_t*) "shell", FIRST_TERM_SHELL);
                    }
                }
                break;
            }
SWITCH_TERMINAL_CONTEXT:
        set_new_page_directory(get_page_directory_for_pid(terminals[get_terminal_of_current_process()].pid));
        return keyboard_state;
    }

    // check if control is pressed
    if(CONTROL_ON(keyboard_state)) {
        if(mapped.result == CLEAR_SCREEN_SHORTCUT) {
            // ctrl + l is pressed then clear screen
            clear_and_reset();
            set_cursor_location(0,0);
            printf_t("391OS> ");            // prints prompt for shell
            printf_t("%s",terminals[terminal_state].stdin);           // print current buffered value
        } else if(mapped.result == ASCII_SIX){
            // ctrl + 6 is pressed then change color
            curr_attribute++;
            if(curr_attribute > MAX_ATTRIB)
                curr_attribute = MIN_ATTRIB;
            change_atribute((curr_back_attribute << 4) | curr_attribute);
        } else if(mapped.result == ASCII_SEVEN){
            // ctrl + 7 is pressed then change color
            curr_back_attribute++;
            if(curr_back_attribute > MAX_BACKGROUD_ATTRIB)
                curr_back_attribute = MIN_BACKGROUD_ATTRIB;
            change_atribute((curr_back_attribute << 4) | curr_attribute);
        }
        set_new_page_directory(get_page_directory_for_pid(terminals[get_terminal_of_current_process()].pid));
        return keyboard_state;
	} else if(BACKSPACE_ON(keyboard_state)){
        // if there are values in terminals[terminal_state].stdin BKSP_CHAR is seen, then delete last char
        if(terminals[terminal_state].stdin_index > 0) {
            putc_kbd(BKSP_CHAR);
            terminals[terminal_state].stdin[--terminals[terminal_state].stdin_index] = NULL_CHAR;
        }
    } else if(mapped.result == NEW_LINE && terminals[terminal_state].stdin_index < KEYBOARD_BUFF_SIZE-1) {
        // play new line on screen
        putc_kbd(NEW_LINE);
        // place NEW_LINE in buffer followed by a NULL_CHAR
        terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = NEW_LINE;
        terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;

        if(terminals[terminal_state].read_waiting == 1){
            // if shell is trying to read, then allow reading
            terminals[terminal_state].allowed_to_read = 1;
        }
        else{
            //else clear terminals[terminal_state].stdin and disallow reading
            memset(terminals[terminal_state].stdin, NULL_CHAR, KEYBOARD_BUFF_SIZE);
            terminals[terminal_state].stdin_index = 0;
            terminals[terminal_state].allowed_to_read = 0;
        }
        set_new_page_directory(get_page_directory_for_pid(terminals[get_terminal_of_current_process()].pid));
        return keyboard_state;
    }

    // stop user from adding greater than 128 keyboard inputs
    if(terminals[terminal_state].stdin_index >= KEYBOARD_BUFF_SIZE-NULL_NL_PADDING) {
        set_new_page_directory(get_page_directory_for_pid(terminals[get_terminal_of_current_process()].pid));
        return keyboard_state;
    }

    //if caps lock is on and shift is not then
    if(CAPS_LOCK_ON(keyboard_state) && !SHIFT_ON(keyboard_state) && !R_SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped)) {
            // if letter, then print capital version
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (mapped.result - ASCII_SHIFT_VAL);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            // if not letter but printable, then print the car directly
            putc_kbd(mapped.result);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
        }
    } else if(SHIFT_ON(keyboard_state) || R_SHIFT_ON(keyboard_state)) {
        if(IS_LETTER_SC(mapped) && !CAPS_LOCK_ON(keyboard_state)) {
            // if shift and caps is not on, then print the capital version of the letter
            putc_kbd(mapped.result - ASCII_SHIFT_VAL);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (mapped.result - ASCII_SHIFT_VAL); // shifting letters is simple
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
        } else if(IS_PRINTABLE_SC(mapped)) {
            // if printable, then check futher characteristics
            if(non_alpha_shift_table[mapped.result] != 0) {
                // if it as char like '1', then print its shited value
                putc_kbd(non_alpha_shift_table[mapped.result]);
                terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (non_alpha_shift_table[mapped.result]);
                terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
            } else {
                // if its not shiftable, then just print directly
                putc_kbd(mapped.result);
                terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (mapped.result); // general printable characters unaffected by caps lock
                terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
            }
        }
    } else {
        if(IS_PRINTABLE_SC(mapped)) {
            // if no spcial conditions and input is printable, then print the input.
            putc_kbd(mapped.result);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index++] = (mapped.result);
            terminals[terminal_state].stdin[terminals[terminal_state].stdin_index] = NULL_CHAR;
        }
    }

    set_new_page_directory(get_page_directory_for_pid(terminals[get_terminal_of_current_process()].pid));
    return keyboard_state; // return current kb state
}
