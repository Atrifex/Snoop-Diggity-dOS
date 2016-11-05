#ifndef SCANCODES_H_
#define SCANCODES_H_

#include "types.h"

#define SCANCODE_TABLE_SIZE 256

/* scancode constants */
#define SCANCODE_MAKE 0
#define SCANCODE_BREAK 1
#define SCANCODE_BREAK_MASK 1

/* marks the bits for the scancode types */
#define SCANCODE_PRINTABLE_BIT 1
#define SCANCODE_LETTER_BIT 2
#define SCANCODE_NUMBER_BIT 3
#define SCANCODE_MODIFIER_BIT 4

/* defines types of scancodes */
#define SCANCODE_PRINTABLE  (1 << SCANCODE_PRINTABLE_BIT)
#define SCANCODE_LETTER     (1 << SCANCODE_LETTER_BIT)
#define SCANCODE_NUMBER     (1 << SCANCODE_NUMBER_BIT)
#define SCANCODE_MODIFIER   (1 << SCANCODE_MODIFIER_BIT)

/* checks if current scancode is of a given type */
#define IS_MAKE_SC(s)      ( (s.flags & SCANCODE_BREAK_MASK) == 0)
#define IS_BREAK_SC(s)     (s.flags & SCANCODE_BREAK_MASK)
#define IS_PRINTABLE_SC(s) ( (s.flags & SCANCODE_PRINTABLE) >> SCANCODE_PRINTABLE_BIT )
#define IS_LETTER_SC(s)    ( (s.flags & SCANCODE_LETTER)    >> SCANCODE_LETTER_BIT  )
#define IS_NUMBER_SC(s)    ( (s.flags & SCANCODE_NUMBER)    >> SCANCODE_NUMBER_BIT  )
#define IS_MODIFIER_SC(s)   ( (s.flags & SCANCODE_MODIFIER)    >> SCANCODE_MODIFIER_BIT  )

// Only concerned w/ 1-byte scancodes (i.e. first 256)
// Multi-byte scancodes (those starting with E0 or E1) will be handled differently
typedef struct scancode_struct {
    uint8_t result;
    uint8_t flags; // bit 0 (LSB): make/break, bit 1: printable/unprintable, bit 2: is letter, bit 3: is number, bit 4: modifier/not modifier
} scancode_t;

/* scancodes and functions used in other files*/
extern scancode_t scancode_table[256];
extern char non_alpha_shift_table[256];
extern void init_scancode_table();

#endif
