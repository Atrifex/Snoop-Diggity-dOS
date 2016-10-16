# Python3 just so ya'll know
# http://www.computer-engineering.org/ps2keyboard/scancodes1.html (source of html)
import sys
import urllib.request
from bs4 import BeautifulSoup as BS

if len(sys.argv) < 2 or len(sys.argv) > 3:
    print("Usage: python3 %s <output file> [source url]" % sys.argv[0])
    exit(1)

source_url = "http://www.computer-engineering.org/ps2keyboard/scancodes1.html" if not len(sys.argv) == 3 else sys.argv[2]

# Source URL
print("Loading from %s" % source_url)

try:
    with urllib.request.urlopen(source_url) as f:
        data = f.read()
except IOError:
    print("Failed to load source URL")
    exit(1)

print("Loaded")
print("Parsing HTML")

parser = BS(data, 'html.parser')

table = parser.find('table', attrs={'cols': '11'}) # lol @ the distinguishing factor
rows = table.find_all('tr')

next_non_printable = 0
# (hex val, key byte val, original description, byte value, is mod, is let, is num, is printable, make/break)
scancodes = []
multibyte_notes = [] # note multibyte things
# row: key/make/break/(blank)/key/make/break/(blank)/key/make/break
for row in rows:
    raw_row = [td.text.strip() for td in row.find_all('td')]
    keys = []
    for x in range(3):
        key = raw_row[x*4].lower().replace('\n', ' ')
        if key == "":
            continue

        # Special cases because being unnecessarily neurotic
        if key == "space": key = " "
        if key == "tab": key = "\t"
        if key == "enter": key = "\n"

        make_code = raw_row[x*4+1].replace("\n", "")
        break_code = raw_row[x*4+2].replace("\n", "")

        if len(make_code) > 2 or len(break_code) > 2:
            multibyte_notes.append("// Key with multibyte scancodes - key \"%s\" - make \"%s\" - break \"%s\"" % (key, make_code, break_code))
            continue

        is_letter = False
        is_number = False
        is_printable = False
        is_modifier = "shft" in key or "ctrl" in key or "gui" in key or "alt" in key

        if len(key) == 1:
            is_printable = True
            is_letter = key.isalpha()
            is_number = key.isdigit()

        if not is_printable:
            byte_val = "%d" % next_non_printable
            next_non_printable += 1
        else:
            byte_val = "%d" % ord(key)

        scancodes.append((make_code, byte_val, key, is_letter, is_number, is_modifier, is_printable, "make"))
        scancodes.append((break_code, byte_val, key, is_letter, is_number, is_modifier, is_printable, "break"))

# Sort by make/break, then by scancode
scancodes.sort(key=lambda val: "%s_%s" % (val[6], val[0]))

buf = []

for sequence, byte_value, original_description, is_letter, is_number, is_modifier, is_printable, make_or_break in scancodes:
    entry = "scancode_table[0x%s]" % sequence
    buf.append("// for \"%s\"" % original_description)
    buf.append("%s.result = %s;" % (entry, byte_value))
    flags = "%s.flags = " % entry

    if make_or_break == "make":
        flags += "SCANCODE_MAKE"
    else: flags += "SCANCODE_BREAK"

    if is_letter: flags += " | SCANCODE_LETTER"
    if is_number: flags += " | SCANCODE_NUMBER"
    if is_printable: flags += " | SCANCODE_PRINTABLE"
    if is_modifier: flags += " | SCANCODE_MODIFIER"

    buf.append(flags + ";")
    buf.append("")

lines = ["%s\n" % l for l in (buf + [''] + multibyte_notes)]
with open(sys.argv[1], 'w') as out:
    out.writelines(lines)

print("Done. Have a great day. Or not, the choice is yours.")
