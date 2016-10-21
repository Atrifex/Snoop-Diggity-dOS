#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "scancodes.h"
#include "types.h"

#define CLEAR_SCREEN_SHORTCUT 'l' // control-L clears screen

// state mask bits
#define SHIFT_MASK 1
#define CONTROL_MASK 2
#define CAPS_LOCK_MASK 4
#define BACKSPACE_MASK 8

// keyboad buffer attributes
#define KEYBOARD_BUFF_SIZE 128
#define EMPTY_SPACE ' '
#define NULL_CHAR '\0'
#define NEW_LINE '\n'

// update state macros
#define TOGGLE_SHIFT(state) (state = state ^ SHIFT_MASK)
#define TOGGLE_CONTROL(state) (state = state ^ CONTROL_MASK)
#define TOGGLE_BACKSPACE(state) (state = state ^ BACKSPACE_MASK)
#define TOGGLE_CAPS(state) (state = state ^ CAPS_LOCK_MASK)

// check state macros
#define CAPS_LOCK_ON(state) (state & CAPS_LOCK_MASK)
#define CONTROL_ON(state) (state & CONTROL_MASK)
#define BACKSPACE_ON(state) (state & BACKSPACE_MASK)
#define SHIFT_ON(state) (state & SHIFT_MASK)

// initializer for a multibyte scancode sequence
#define MB_SEQ_INIT 0xE0

// shift value for making a lowercase letter uppercase
#define ASCII_SHIFT_VAL ('a' - 'A')

// keyboard communication packets
#define MAKE_RELEASE_WORD 0xF8
#define KEYBOARD_ACK 0xFA
#define KEYBOARD_RESEND 0xFE

#define PS2_CONTROL_PORT 0x64
#define PS2_DISABLE_KBD 0xAD
#define PS2_ENABLE_KBD 0xAE

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CONTROL_PORT 0x60

#define KEYBOARD_LINE_NO 1

// modifier keys
#define CONTROL_PRESS 0x1D
#define CONTROL_RELEASE 0x9D

#define SHIFT_PRESS 0x2A
#define SHIFT_RELEASE 0xAA

#define CAPS_LOCK_PRESS 0x3A
#define CAPS_LOCK_RELEASE 0xBA

#define BACKSPACE_PRESS 0x0E
#define BACKSPACE_RELEASE 0x8E

/* Process the sent scancode after an interrupt */
extern unsigned long process_sent_scancode();
/* Initialize the keyboard device */
extern void init_kbd();

#endif
