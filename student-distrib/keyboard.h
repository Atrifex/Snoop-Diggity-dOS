#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "scancodes.h"
#include "types.h"

#define KEYBOARD_PORT 0x61
#define MAKE_RELEASE_WORD 0xFC
#define CONTROL_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x60

#define KEYBOARD_LINE_NO 1

// modifier keys
#define CONTROL_PRESS 0x1D
#define CONTROL_RELEASE 0x9D

#define SHIFT_PRESS 0x2A
#define SHIFT_RELEASE 0xAA

#define CAPS_LOCK_PRESS 0x3A
#define CAPS_LOCK_RELEASE 0xBA

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

/* Process the sent scancode after an interrupt */
extern unsigned long process_sent_scancode();
/* Initialize the keyboard device */
extern void init_kbd();

#endif
