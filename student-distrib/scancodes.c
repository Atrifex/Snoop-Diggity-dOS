#include "scancodes.h"
#include "lib.h"

scancode_t scancode_table[256];
char non_alpha_shift_table[256];

/*
 * init_scancode_table()
 * DESCRIPTION: Initializes the scancode table and the shift table
 * INPUT: none
 * OUTPUTS: none
 * RETURN VALUE: none
 * SIDE EFFECTS: Updates scancode_table, non_alpha_shift_table
*/
void init_scancode_table()
{
    // set-up non alpha shift table first
    memset((void*) non_alpha_shift_table, 0, sizeof(non_alpha_shift_table));
    non_alpha_shift_table['1'] = '!';
    non_alpha_shift_table['2'] = '@';
    non_alpha_shift_table['3'] = '#';
    non_alpha_shift_table['4'] = '$';
    non_alpha_shift_table['5'] = '%';
    non_alpha_shift_table['6'] = '^';
    non_alpha_shift_table['7'] = '&';
    non_alpha_shift_table['8'] = '*';
    non_alpha_shift_table['9'] = '(';
    non_alpha_shift_table['0'] = ')';
    non_alpha_shift_table['-'] = '_';
    non_alpha_shift_table['='] = '+';
    non_alpha_shift_table['['] = '{';
    non_alpha_shift_table[']'] = '}';
    non_alpha_shift_table['\\'] = '|';
    non_alpha_shift_table['\''] = '"';
    non_alpha_shift_table[';'] = ':';
    non_alpha_shift_table[','] = '<';
    non_alpha_shift_table['.'] = '>';
    non_alpha_shift_table['/'] = '?';
    non_alpha_shift_table['`'] = '~';

    // generated code

    // for "esc"
    scancode_table[0x01].result = 12;
    scancode_table[0x01].flags = SCANCODE_MAKE;

    // for "bksp"
    scancode_table[0x0E].result = 0;
    scancode_table[0x0E].flags = SCANCODE_MAKE;

    // for "l ctrl"
    scancode_table[0x1D].result = 3;
    scancode_table[0x1D].flags = SCANCODE_MAKE | SCANCODE_MODIFIER;

    // for "l shft"
    scancode_table[0x2A].result = 2;
    scancode_table[0x2A].flags = SCANCODE_MAKE | SCANCODE_MODIFIER;

    // for "r shft"
    scancode_table[0x36].result = 6;
    scancode_table[0x36].flags = SCANCODE_MAKE | SCANCODE_MODIFIER;

    // for "kp *"
    scancode_table[0x37].result = 7;
    scancode_table[0x37].flags = SCANCODE_MAKE;

    // for "l alt"
    scancode_table[0x38].result = 5;
    scancode_table[0x38].flags = SCANCODE_MAKE | SCANCODE_MODIFIER;

    // for "caps"
    scancode_table[0x3A].result = 1;
    scancode_table[0x3A].flags = SCANCODE_MAKE;

    // for "f1"
    scancode_table[0x3B].result = 14;
    scancode_table[0x3B].flags = SCANCODE_MAKE;

    // for "f2"
    scancode_table[0x3C].result = 16;
    scancode_table[0x3C].flags = SCANCODE_MAKE;

    // for "f3"
    scancode_table[0x3D].result = 18;
    scancode_table[0x3D].flags = SCANCODE_MAKE;

    // for "f4"
    scancode_table[0x3E].result = 20;
    scancode_table[0x3E].flags = SCANCODE_MAKE;

    // for "f5"
    scancode_table[0x3F].result = 22;
    scancode_table[0x3F].flags = SCANCODE_MAKE;

    // for "f6"
    scancode_table[0x40].result = 24;
    scancode_table[0x40].flags = SCANCODE_MAKE;

    // for "f7"
    scancode_table[0x41].result = 26;
    scancode_table[0x41].flags = SCANCODE_MAKE;

    // for "f8"
    scancode_table[0x42].result = 28;
    scancode_table[0x42].flags = SCANCODE_MAKE;

    // for "f9"
    scancode_table[0x43].result = 30;
    scancode_table[0x43].flags = SCANCODE_MAKE;

    // for "f10"
    scancode_table[0x44].result = 31;
    scancode_table[0x44].flags = SCANCODE_MAKE;

    // for "num"
    scancode_table[0x45].result = 4;
    scancode_table[0x45].flags = SCANCODE_MAKE;

    // for "scroll"
    scancode_table[0x46].result = 34;
    scancode_table[0x46].flags = SCANCODE_MAKE;

    // for "kp 7"
    scancode_table[0x47].result = 25;
    scancode_table[0x47].flags = SCANCODE_MAKE;

    // for "kp 8"
    scancode_table[0x48].result = 27;
    scancode_table[0x48].flags = SCANCODE_MAKE;

    // for "kp 9"
    scancode_table[0x49].result = 29;
    scancode_table[0x49].flags = SCANCODE_MAKE;

    // for "kp -"
    scancode_table[0x4A].result = 8;
    scancode_table[0x4A].flags = SCANCODE_MAKE;

    // for "kp 4"
    scancode_table[0x4B].result = 19;
    scancode_table[0x4B].flags = SCANCODE_MAKE;

    // for "kp 5"
    scancode_table[0x4C].result = 21;
    scancode_table[0x4C].flags = SCANCODE_MAKE;

    // for "kp 6"
    scancode_table[0x4D].result = 23;
    scancode_table[0x4D].flags = SCANCODE_MAKE;

    // for "kp +"
    scancode_table[0x4E].result = 9;
    scancode_table[0x4E].flags = SCANCODE_MAKE;

    // for "kp 1"
    scancode_table[0x4F].result = 13;
    scancode_table[0x4F].flags = SCANCODE_MAKE;

    // for "kp 2"
    scancode_table[0x50].result = 15;
    scancode_table[0x50].flags = SCANCODE_MAKE;

    // for "kp 3"
    scancode_table[0x51].result = 17;
    scancode_table[0x51].flags = SCANCODE_MAKE;

    // for "kp 0"
    scancode_table[0x52].result = 11;
    scancode_table[0x52].flags = SCANCODE_MAKE;

    // for "kp ."
    scancode_table[0x53].result = 10;
    scancode_table[0x53].flags = SCANCODE_MAKE;

    // for "f11"
    scancode_table[0x57].result = 32;
    scancode_table[0x57].flags = SCANCODE_MAKE;

    // for "f12"
    scancode_table[0x58].result = 33;
    scancode_table[0x58].flags = SCANCODE_MAKE;

    // for "esc"
    scancode_table[0x81].result = 12;
    scancode_table[0x81].flags = SCANCODE_BREAK;

    // for "bksp"
    scancode_table[0x8E].result = 0;
    scancode_table[0x8E].flags = SCANCODE_BREAK;

    // for "l ctrl"
    scancode_table[0x9D].result = 3;
    scancode_table[0x9D].flags = SCANCODE_BREAK | SCANCODE_MODIFIER;

    // for "l shft"
    scancode_table[0xAA].result = 2;
    scancode_table[0xAA].flags = SCANCODE_BREAK | SCANCODE_MODIFIER;

    // for "r shft"
    scancode_table[0xB6].result = 6;
    scancode_table[0xB6].flags = SCANCODE_BREAK | SCANCODE_MODIFIER;

    // for "kp *"
    scancode_table[0xB7].result = 7;
    scancode_table[0xB7].flags = SCANCODE_BREAK;

    // for "l alt"
    scancode_table[0xB8].result = 5;
    scancode_table[0xB8].flags = SCANCODE_BREAK | SCANCODE_MODIFIER;

    // for "caps"
    scancode_table[0xBA].result = 1;
    scancode_table[0xBA].flags = SCANCODE_BREAK;

    // for "f1"
    scancode_table[0xBB].result = 14;
    scancode_table[0xBB].flags = SCANCODE_BREAK;

    // for "f2"
    scancode_table[0xBC].result = 16;
    scancode_table[0xBC].flags = SCANCODE_BREAK;

    // for "f3"
    scancode_table[0xBD].result = 18;
    scancode_table[0xBD].flags = SCANCODE_BREAK;

    // for "f4"
    scancode_table[0xBE].result = 20;
    scancode_table[0xBE].flags = SCANCODE_BREAK;

    // for "f5"
    scancode_table[0xBF].result = 22;
    scancode_table[0xBF].flags = SCANCODE_BREAK;

    // for "f6"
    scancode_table[0xC0].result = 24;
    scancode_table[0xC0].flags = SCANCODE_BREAK;

    // for "f7"
    scancode_table[0xC1].result = 26;
    scancode_table[0xC1].flags = SCANCODE_BREAK;

    // for "f8"
    scancode_table[0xC2].result = 28;
    scancode_table[0xC2].flags = SCANCODE_BREAK;

    // for "f9"
    scancode_table[0xC3].result = 30;
    scancode_table[0xC3].flags = SCANCODE_BREAK;

    // for "f10"
    scancode_table[0xC4].result = 31;
    scancode_table[0xC4].flags = SCANCODE_BREAK;

    // for "num"
    scancode_table[0xC5].result = 4;
    scancode_table[0xC5].flags = SCANCODE_BREAK;

    // for "scroll"
    scancode_table[0xC6].result = 34;
    scancode_table[0xC6].flags = SCANCODE_BREAK;

    // for "kp 7"
    scancode_table[0xC7].result = 25;
    scancode_table[0xC7].flags = SCANCODE_BREAK;

    // for "kp 8"
    scancode_table[0xC8].result = 27;
    scancode_table[0xC8].flags = SCANCODE_BREAK;

    // for "kp 9"
    scancode_table[0xC9].result = 29;
    scancode_table[0xC9].flags = SCANCODE_BREAK;

    // for "kp -"
    scancode_table[0xCA].result = 8;
    scancode_table[0xCA].flags = SCANCODE_BREAK;

    // for "kp 4"
    scancode_table[0xCB].result = 19;
    scancode_table[0xCB].flags = SCANCODE_BREAK;

    // for "kp 5"
    scancode_table[0xCC].result = 21;
    scancode_table[0xCC].flags = SCANCODE_BREAK;

    // for "kp 6"
    scancode_table[0xCD].result = 23;
    scancode_table[0xCD].flags = SCANCODE_BREAK;

    // for "kp +"
    scancode_table[0xCE].result = 9;
    scancode_table[0xCE].flags = SCANCODE_BREAK;

    // for "kp 1"
    scancode_table[0xCF].result = 13;
    scancode_table[0xCF].flags = SCANCODE_BREAK;

    // for "kp 2"
    scancode_table[0xD0].result = 15;
    scancode_table[0xD0].flags = SCANCODE_BREAK;

    // for "kp 3"
    scancode_table[0xD1].result = 17;
    scancode_table[0xD1].flags = SCANCODE_BREAK;

    // for "kp 0"
    scancode_table[0xD2].result = 11;
    scancode_table[0xD2].flags = SCANCODE_BREAK;

    // for "kp ."
    scancode_table[0xD3].result = 10;
    scancode_table[0xD3].flags = SCANCODE_BREAK;

    // for "f11"
    scancode_table[0xD7].result = 32;
    scancode_table[0xD7].flags = SCANCODE_BREAK;

    // for "f12"
    scancode_table[0xD8].result = 33;
    scancode_table[0xD8].flags = SCANCODE_BREAK;

    // for "1"
    scancode_table[0x02].result = 49;
    scancode_table[0x02].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "2"
    scancode_table[0x03].result = 50;
    scancode_table[0x03].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "3"
    scancode_table[0x04].result = 51;
    scancode_table[0x04].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "4"
    scancode_table[0x05].result = 52;
    scancode_table[0x05].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "5"
    scancode_table[0x06].result = 53;
    scancode_table[0x06].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "6"
    scancode_table[0x07].result = 54;
    scancode_table[0x07].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "7"
    scancode_table[0x08].result = 55;
    scancode_table[0x08].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "8"
    scancode_table[0x09].result = 56;
    scancode_table[0x09].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "9"
    scancode_table[0x0A].result = 57;
    scancode_table[0x0A].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "0"
    scancode_table[0x0B].result = 48;
    scancode_table[0x0B].flags = SCANCODE_MAKE | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "-"
    scancode_table[0x0C].result = 45;
    scancode_table[0x0C].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "="
    scancode_table[0x0D].result = 61;
    scancode_table[0x0D].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "	"
    scancode_table[0x0F].result = 9;
    scancode_table[0x0F].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "q"
    scancode_table[0x10].result = 113;
    scancode_table[0x10].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "w"
    scancode_table[0x11].result = 119;
    scancode_table[0x11].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "e"
    scancode_table[0x12].result = 101;
    scancode_table[0x12].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "r"
    scancode_table[0x13].result = 114;
    scancode_table[0x13].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "t"
    scancode_table[0x14].result = 116;
    scancode_table[0x14].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "y"
    scancode_table[0x15].result = 121;
    scancode_table[0x15].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "u"
    scancode_table[0x16].result = 117;
    scancode_table[0x16].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "i"
    scancode_table[0x17].result = 105;
    scancode_table[0x17].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "o"
    scancode_table[0x18].result = 111;
    scancode_table[0x18].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "p"
    scancode_table[0x19].result = 112;
    scancode_table[0x19].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "["
    scancode_table[0x1A].result = 91;
    scancode_table[0x1A].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "]"
    scancode_table[0x1B].result = 93;
    scancode_table[0x1B].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for " " "
    scancode_table[0x1C].result = 10;
    scancode_table[0x1C].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "a"
    scancode_table[0x1E].result = 97;
    scancode_table[0x1E].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "s"
    scancode_table[0x1F].result = 115;
    scancode_table[0x1F].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "d"
    scancode_table[0x20].result = 100;
    scancode_table[0x20].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "f"
    scancode_table[0x21].result = 102;
    scancode_table[0x21].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "g"
    scancode_table[0x22].result = 103;
    scancode_table[0x22].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "h"
    scancode_table[0x23].result = 104;
    scancode_table[0x23].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "j"
    scancode_table[0x24].result = 106;
    scancode_table[0x24].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "k"
    scancode_table[0x25].result = 107;
    scancode_table[0x25].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "l"
    scancode_table[0x26].result = 108;
    scancode_table[0x26].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for ";"
    scancode_table[0x27].result = 59;
    scancode_table[0x27].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "'"
    scancode_table[0x28].result = 39;
    scancode_table[0x28].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "`"
    scancode_table[0x29].result = 96;
    scancode_table[0x29].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "\"
    scancode_table[0x2B].result = 92;
    scancode_table[0x2B].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "z"
    scancode_table[0x2C].result = 122;
    scancode_table[0x2C].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "x"
    scancode_table[0x2D].result = 120;
    scancode_table[0x2D].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "c"
    scancode_table[0x2E].result = 99;
    scancode_table[0x2E].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "v"
    scancode_table[0x2F].result = 118;
    scancode_table[0x2F].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "b"
    scancode_table[0x30].result = 98;
    scancode_table[0x30].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "n"
    scancode_table[0x31].result = 110;
    scancode_table[0x31].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "m"
    scancode_table[0x32].result = 109;
    scancode_table[0x32].flags = SCANCODE_MAKE | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for ","
    scancode_table[0x33].result = 44;
    scancode_table[0x33].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "."
    scancode_table[0x34].result = 46;
    scancode_table[0x34].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "/"
    scancode_table[0x35].result = 47;
    scancode_table[0x35].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for " "
    scancode_table[0x39].result = 32;
    scancode_table[0x39].flags = SCANCODE_MAKE | SCANCODE_PRINTABLE;

    // for "1"
    scancode_table[0x82].result = 49;
    scancode_table[0x82].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "2"
    scancode_table[0x83].result = 50;
    scancode_table[0x83].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "3"
    scancode_table[0x84].result = 51;
    scancode_table[0x84].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "4"
    scancode_table[0x85].result = 52;
    scancode_table[0x85].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "5"
    scancode_table[0x86].result = 53;
    scancode_table[0x86].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "6"
    scancode_table[0x87].result = 54;
    scancode_table[0x87].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "7"
    scancode_table[0x88].result = 55;
    scancode_table[0x88].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "`"
    scancode_table[0x89].result = 96;
    scancode_table[0x89].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "8"
    scancode_table[0x89].result = 56;
    scancode_table[0x89].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "9"
    scancode_table[0x8A].result = 57;
    scancode_table[0x8A].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "0"
    scancode_table[0x8B].result = 48;
    scancode_table[0x8B].flags = SCANCODE_BREAK | SCANCODE_NUMBER | SCANCODE_PRINTABLE;

    // for "-"
    scancode_table[0x8C].result = 45;
    scancode_table[0x8C].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "="
    scancode_table[0x8D].result = 61;
    scancode_table[0x8D].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "	"
    scancode_table[0x8F].result = 9;
    scancode_table[0x8F].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "q"
    scancode_table[0x90].result = 113;
    scancode_table[0x90].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "w"
    scancode_table[0x91].result = 119;
    scancode_table[0x91].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "e"
    scancode_table[0x92].result = 101;
    scancode_table[0x92].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "r"
    scancode_table[0x93].result = 114;
    scancode_table[0x93].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "t"
    scancode_table[0x94].result = 116;
    scancode_table[0x94].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "y"
    scancode_table[0x95].result = 121;
    scancode_table[0x95].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "u"
    scancode_table[0x96].result = 117;
    scancode_table[0x96].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "i"
    scancode_table[0x97].result = 105;
    scancode_table[0x97].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "o"
    scancode_table[0x98].result = 111;
    scancode_table[0x98].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "p"
    scancode_table[0x99].result = 112;
    scancode_table[0x99].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "["
    scancode_table[0x9A].result = 91;
    scancode_table[0x9A].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "]"
    scancode_table[0x9B].result = 93;
    scancode_table[0x9B].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for ""
    scancode_table[0x9C].result = 10;
    scancode_table[0x9C].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "a"
    scancode_table[0x9E].result = 97;
    scancode_table[0x9E].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "s"
    scancode_table[0x9F].result = 115;
    scancode_table[0x9F].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "d"
    scancode_table[0xA0].result = 100;
    scancode_table[0xA0].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "f"
    scancode_table[0xA1].result = 102;
    scancode_table[0xA1].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "g"
    scancode_table[0xA2].result = 103;
    scancode_table[0xA2].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "h"
    scancode_table[0xA3].result = 104;
    scancode_table[0xA3].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "j"
    scancode_table[0xA4].result = 106;
    scancode_table[0xA4].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "k"
    scancode_table[0xA5].result = 107;
    scancode_table[0xA5].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "l"
    scancode_table[0xA6].result = 108;
    scancode_table[0xA6].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for ";"
    scancode_table[0xA7].result = 59;
    scancode_table[0xA7].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "'"
    scancode_table[0xA8].result = 39;
    scancode_table[0xA8].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "\"
    scancode_table[0xAB].result = 92;
    scancode_table[0xAB].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "z"
    scancode_table[0xAC].result = 122;
    scancode_table[0xAC].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "x"
    scancode_table[0xAD].result = 120;
    scancode_table[0xAD].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "c"
    scancode_table[0xAE].result = 99;
    scancode_table[0xAE].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "v"
    scancode_table[0xAF].result = 118;
    scancode_table[0xAF].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "b"
    scancode_table[0xB0].result = 98;
    scancode_table[0xB0].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "n"
    scancode_table[0xB1].result = 110;
    scancode_table[0xB1].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for "m"
    scancode_table[0xB2].result = 109;
    scancode_table[0xB2].flags = SCANCODE_BREAK | SCANCODE_LETTER | SCANCODE_PRINTABLE;

    // for ","
    scancode_table[0xB3].result = 44;
    scancode_table[0xB3].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "."
    scancode_table[0xB4].result = 46;
    scancode_table[0xB4].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for "/"
    scancode_table[0xB5].result = 47;
    scancode_table[0xB5].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;

    // for " "
    scancode_table[0xB9].result = 32;
    scancode_table[0xB9].flags = SCANCODE_BREAK | SCANCODE_PRINTABLE;


    // Key with multibyte scancodes - key "insert" - make "E0,52" - break "E0,D2"
    // Key with multibyte scancodes - key "home" - make "E0,47" - break "E0,97"
    // Key with multibyte scancodes - key "pg up" - make "E0,49" - break "E0,C9"
    // Key with multibyte scancodes - key "delete" - make "E0,53" - break "E0,D3"
    // Key with multibyte scancodes - key "end" - make "E0,4F" - break "E0,CF"
    // Key with multibyte scancodes - key "pg dn" - make "E0,51" - break "E0,D1"
    // Key with multibyte scancodes - key "u arrow" - make "E0,48" - break "E0,C8"
    // Key with multibyte scancodes - key "l arrow" - make "E0,4B" - break "E0,CB"
    // Key with multibyte scancodes - key "d arrow" - make "E0,50" - break "E0,D0"
    // Key with multibyte scancodes - key "r arrow" - make "E0,4D" - break "E0,CD"
    // Key with multibyte scancodes - key "l gui" - make "E0,5B" - break "E0,DB"
    // Key with multibyte scancodes - key "kp /" - make "E0,35" - break "E0,B5"
    // Key with multibyte scancodes - key "r ctrl" - make "E0,1D" - break "E0,9D"
    // Key with multibyte scancodes - key "r gui" - make "E0,5C" - break "E0,DC"
    // Key with multibyte scancodes - key "r alt" - make "E0,38" - break "E0,B8"
    // Key with multibyte scancodes - key "kp en" - make "E0,1C" - break "E0,9C"
    // Key with multibyte scancodes - key "apps" - make "E0,5D" - break "E0,DD"
    // Key with multibyte scancodes - key "prnt  scrn" - make "E0,2A, E0,37" - break "E0,B7, E0,AA"
    // Key with multibyte scancodes - key "pause" - make "E1,1D,45 E1,9D,C5" - break "-NONE-"
}
