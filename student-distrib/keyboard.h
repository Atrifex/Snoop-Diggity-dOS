#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_PORT 0x61
#define MAKE_RELEASE_WORD 0xFC
#define CONTROL_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x60

#define KEYBOARD_LINE_NO 1

// modifier keys
#define CAPS_LOCK_PRESS 
#define LEFT_SHIFT_PRESS
#define RIGHT_SHIFT_PRESS
#define LEFT_CTL_PRESS
#define RIGHT_CTL_PRESS 

#define CAPS_LOCK_RELEASE 
#define LEFT_SHIFT_RELEASE 
#define RIGHT_SHIFT_RELEASE 
#define LEFT_CTL_RELEASE 
#define RIGHT_CTL_RELEASE 

#define CHAR_SHIFT 6


#define CTL_MAP 2
#define SHIFT_MAP 1
#define NORMAL_MAP 0
#define CAP_LOCK_STATE 0x02
#define SHIFT_STATE 0x01
#define CTL_STATE 0x04
#define NUMB_KEYS 128
#define NUMB_MAPS 3
#define NUMB_STATES 8


extern unsigned long display_char();

extern void init_kbd();
char get_char();

#endif
