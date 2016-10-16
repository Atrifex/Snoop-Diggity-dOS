#ifndef PS2_H
#define PS2_H

#define USE_PS2_CONTROLLER 0 // Are we actually doing anything with the controller?

// Ports for the keyboard controller
#define PS2_BUFFER_PORT 0x60
#define PS2_STATUS_CMD_PORT 0x64

// Commands to send to the keyboard controller
#define DISABLE_KBD 0xAD
#define DISABLE_MOUSE 0xA7
#define ENABLE_KBD 0xAE
#define ENABLE_MOUSE 0xA8
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60
#define CONTROLLER_SELF_TEST 0xAA
#define CONTROLLER_OKAY_RESPONSE 0xFA // Need to check- wiki unsure
#define KEYBOARD_INTERFACE 0xAB
#define MOUSE_INTERFACE 0xA9

// Bitmasking
#define ISOLATE_BIT_5 0x020
#define ISOLATE_BIT_4 0x010
#define ISOLATE_BIT_1 0x02
#define ISOLATE_BIT_0 0x01
#define CLEAR_BIT_0 0xFFFFFFFE
#define CLEAR_BIT_1 0xFFFFFFFD
#define CLEAR_BIT_6 0xFFFFFFBF
#define SET_BIT_0 0x00000001
#define SET_BIT_1 0x00000002

#if USE_PS2_CONTROLLER
extern void init_ps2();
#endif

#endif
