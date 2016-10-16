#ifndef KEYBOARD_H
#define KEYBOARD_H
#define KEYBOARD_PORT 0x21
#define MAKE_RELEASE_WORD 0xFC
#define CONTROL_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x60
#define CAPS_LOCK_PRESS 0x00
#define CAPS_LOCK_RELEASE 0x00
#define SHIFT_PRESS 0x00
#define SHIFT_RELEASE 0x00
#define CTL_PRESS 0x00
#define CTL_RELEASE 0x00
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

unsigned long choose_map[NUMB_STATES] = {NORMAL_MAP, SHIFT_MAP, SHIFT_MAP, NORMAL_MAP, CTL_MAP, CTL_MAP, CTL_MAP, CTL_MAP};
unsigned char keyboard_normalmap[NUMB_KEYS] = {}; 
unsigned char keyboard_shiftmap[NUMB_KEYS] =  {}; 
unsigned char keyboard_controlmap[NUMB_KEYS] = { }; 
unsigned char* keyboard_map_select[NUMB_MAPS] = {keyboard_normalmap, keyboard_shiftmap, keyboard_controlmap};
extern char display_char(unsigned long state);
extern void init_kbd();
char get_char();

#endif
