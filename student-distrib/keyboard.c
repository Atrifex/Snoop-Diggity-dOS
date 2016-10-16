#include "keyboard.h"
#include "lib.h"
#include "i8259.h"

unsigned long keyboard_state = 0;

unsigned long choose_map[NUMB_STATES] = {NORMAL_MAP, SHIFT_MAP, SHIFT_MAP, NORMAL_MAP, CTL_MAP, CTL_MAP, CTL_MAP, CTL_MAP};
// unsigned char keyboard_normalmap[NUMB_KEYS] = {}; 
// unsigned char keyboard_shiftmap[NUMB_KEYS] =  {}; 
// unsigned char keyboard_controlmap[NUMB_KEYS] = { }; 
//unsigned char* keyboard_map_select[NUMB_MAPS] = {keyboard_normalmap, keyboard_shiftmap, keyboard_controlmap};

#define BYTE_ZERO_MASK 0xFF

#define BYTE_ONE_MASK  0xFF00
#define BYTE_ONE_SHIFT 8

#define BYTE_TWO_MASK 0xFF0000
#define BYTE_TWO_SHIFT 16

#define BYTE_THREE_MASK 0xFF000000
#define BYTE_THREE_SHIFT 24

/*
 * get_char
 * DESCRIPTION: Retrieves scancode(s) from control data port
 * INPUT: none.
 * OUTPUTS: scancode(s) present in control data port
 * RETURN VALUE: none.
 * SIDE EFFECTS: Drains keyboard input
*/
uint32_t get_char()
{
    uint8_t a;
    uint8_t result[4] = {0, 0, 0, 0};
    uint8_t seen_non_zero = 0;
    int index = 0;
    
    while(1)
    {
        if(a != inb(CONTROL_DATA_PORT)) {
            a = inb(CONTROL_DATA_PORT);
            while(a != 0) {
                seen_non_zero = 1;
                result[index] = a;
                index++;
            }
                
            if(seen_non_zero) {
                // pack the result
                return result[0] & (result[1] << BYTE_ONE_SHIFT) & (result[2] << BYTE_TWO_SHIFT) & (result[3] << BYTE_THREE_SHIFT);
            }
        }
    }
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
    init_scancode_table();
    outb(MAKE_RELEASE_WORD, KEYBOARD_PORT);
    enable_irq(KEYBOARD_LINE_NO);
}

/*
 * display_char
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
    unsigned long flags;  //make critical so keyboard_states are able remain correct
    cli_and_save(flags); // save flags and halt interupts for critical section
    uint32_t temp = get_char();
    uint8_t scancode[4] = {0,0,0,0};
    scancode[0] = (uint8_t) ( temp & BYTE_ZERO_MASK );
    scancode[1] = (uint8_t) ( ( temp & BYTE_ONE_MASK ) >> BYTE_ONE_SHIFT );
    scancode[2] = (uint8_t) ( ( temp & BYTE_TWO_MASK ) >> BYTE_TWO_SHIFT );
    scancode[3] = (uint8_t) ( ( temp & BYTE_THREE_MASK ) >> BYTE_THREE_SHIFT );

    scancode_t mapped;
    
    /*determines map to use either controlmap, shiftmap, or normal map
    control has highest priority, and therefore goes to control map
    */
    unsigned long selected_map = choose_map[keyboard_state];
    switch(scancode[0]) {
        case (CAPS_LOCK_PRESS):
            keyboard_state = (keyboard_state ^ CAP_LOCK_STATE);
            break;
        case (CAPS_LOCK_RELEASE):
            //dont care about this, return current keyboard_state with null
            break;
        case (SHIFT_PRESS):
            keyboard_state = (keyboard_state ^ SHIFT_STATE);
            break;
        case (SHIFT_RELEASE):
            keyboard_state = (keyboard_state ^ SHIFT_STATE);
            break;
        case (CONTROL_PRESS):
            keyboard_state = (keyboard_state ^ CTL_STATE);
            break;
        case (CONTROL_RELEASE):
            keyboard_state = (keyboard_state ^ CTL_STATE);
            break;
            //assuming nothing goes wrong for ctl and shift press sets corresponding keyboard_state bits to 1
            //release sets corresponding keyboard_state bits to 0
        default:
            // char adjusted = (keyboard_map_select[selected_map])[a];
            // unsigned long adjusted_char = adjusted << CHAR_SHIFT; //
            if(selected_map == NORMAL_MAP) {
                mapped = scancode_table[scancode[0]];
                if(IS_PRINTABLE_SC(mapped)) {
                    putc(mapped.result);
                }
            }
            break;
    }
    
    restore_flags(flags); // restore our flags
    return keyboard_state; // return current kb state
}
