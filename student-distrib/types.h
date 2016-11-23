/* types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _TYPES_H
#define _TYPES_H

#define NULL 0

#ifndef ASM


// return codes
#define SUCCESS 0
#define ERROR -1
#define FAILURE -1
#define TRUE 1
#define FALSE 0

// globally necessary constants
#define STDIN 0
#define STDOUT 1
#define EMPTY_SPACE ' '
#define NULL_CHAR '\0'
#define NEW_LINE '\n'
#define CARRIAGE_RETURN '\r' // mac os 7 compat
#define BKSP_CHAR 8

 #define asmlinkage __attribute__((regparm(0)))

/* Types defined here just like in <stdint.h> */
typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

typedef int ssize_t;
typedef unsigned int size_t;

#define LITERAL_4MB 0x400000
#define LITERAL_4KB 0x00001000
#define LITERAL_8KB  (LITERAL_4KB*2)
#define LITERAL_12KB (LITERAL_4KB*3)

#endif /* ASM */

#endif /* _TYPES_H */
