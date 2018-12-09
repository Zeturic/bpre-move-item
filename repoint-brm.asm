.definelabel bottom_right_menu_old, 0x0845A618
.definelabel bottom_right_menu_new, 0x08C0A618

num_entries_old equ 0x1E
num_addl_entries equ 4
flashid_old equ 0x12

// -----------------------------------------------------------------------------

sizeof equ 8

// -----------------------------------------------------------------------------

.gba
.thumb

.create "test.gba", 0x08000000
.import "rom.gba"

.include "macros.s"

// -----------------------------------------------------------------------------

// populate new table
.org bottom_right_menu_new
importm "rom.gba", bottom_right_menu_old, flashid_old * sizeof
.fill num_addl_entries * sizeof, 0xFE
importm "rom.gba", bottom_right_menu_old + flashid_old * sizeof, (num_entries_old - flashid_old) * sizeof

// -----------------------------------------------------------------------------

// update pointers to the table
updatebrmptr 0x08120F74
updatebrmptr 0x08121FB8
updatebrmptr 0x08122D18
updatebrmptr 0x08122D48

// -----------------------------------------------------------------------------

// clear old table
.org bottom_right_menu_old
.fill num_entries_old * sizeof, 0xFF

// -----------------------------------------------------------------------------

// the assumption of field moves starting at 0x12 is in various places

.org 0x081245BC
sub r0, flashid_old + num_addl_entries

.org 0x08122146
cmp r5, flashid_old + num_addl_entries - 1

.org 0x0812219E
sub r0, flashid_old + num_addl_entries

.org 0x08122A6A
add r2, flashid_old + num_addl_entries

.org 0x08121F36
cmp r0, flashid_old + num_addl_entries - 1

.close