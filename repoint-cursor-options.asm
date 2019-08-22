.definelabel free_space, 0x08800000

NUM_FIELD_MOVE_CURSOR_OPTIONS equ 12

NUM_REGULAR_CURSOR_OPTIONS_OLD equ 18
NUM_ADDITIONAL_REGULAR_CURSOR_OPTIONS equ 1

// -----------------------------------------------------------------------------

.definelabel sCursorOptionsOld, readu32("rom.gba", 0x08120F74 & 0x1FFFFFF)

.if sCursorOptionsOld == free_space
    .error "The source and destination of the repointing are the same."
.endif

// -----------------------------------------------------------------------------

NUM_REGULAR_CURSOR_OPTIONS_NEW equ (NUM_REGULAR_CURSOR_OPTIONS_OLD + NUM_ADDITIONAL_REGULAR_CURSOR_OPTIONS)

SIZEOF_CURSOR_OPTION equ 8

MENU_FIELD_MOVES_OLD equ NUM_REGULAR_CURSOR_OPTIONS_OLD
MENU_FIELD_MOVES_NEW equ NUM_REGULAR_CURSOR_OPTIONS_NEW

// -----------------------------------------------------------------------------

.include "macros.s"

.gba
.open "rom.gba", 0x08000000

// -----------------------------------------------------------------------------

// populate the new cursor options table using the contents of the old one

.org free_space
sCursorOptionsNew:
    incbinm "rom.gba", sCursorOptionsOld, NUM_REGULAR_CURSOR_OPTIONS_OLD * SIZEOF_CURSOR_OPTION
    .fill NUM_ADDITIONAL_REGULAR_CURSOR_OPTIONS * SIZEOF_CURSOR_OPTION, 0
    incbinm "rom.gba", sCursorOptionsOld + NUM_REGULAR_CURSOR_OPTIONS_OLD * SIZEOF_CURSOR_OPTION, NUM_FIELD_MOVE_CURSOR_OPTIONS * SIZEOF_CURSOR_OPTION

// -----------------------------------------------------------------------------

// delete the old cursor options table
.org sCursorOptionsOld
.fill (NUM_REGULAR_CURSOR_OPTIONS_OLD + NUM_FIELD_MOVE_CURSOR_OPTIONS) * SIZEOF_CURSOR_OPTION, 0xFF

// -----------------------------------------------------------------------------

// update pointers to the old cursor options table
safeupdateptr 0x08120F74, sCursorOptionsOld, sCursorOptionsNew
safeupdateptr 0x08121FB8, sCursorOptionsOld, sCursorOptionsNew
safeupdateptr 0x08122D18, sCursorOptionsOld, sCursorOptionsNew
safeupdateptr 0x08122D48, sCursorOptionsOld, sCursorOptionsNew

// -----------------------------------------------------------------------------

// the assumption of field moves starting at 0x12 is in various places

.org 0x081245BC
    sub r0, MENU_FIELD_MOVES_NEW

.org 0x08122146
    cmp r5, MENU_FIELD_MOVES_NEW - 1

.org 0x0812219E
    sub r0, MENU_FIELD_MOVES_NEW

.org 0x08122A6A
    add r2, MENU_FIELD_MOVES_NEW

.org 0x08121F36
    cmp r0, MENU_FIELD_MOVES_NEW - 1

// -----------------------------------------------------------------------------

.close

.if sCursorOptionsOld != free_space
    .notice "sCursorOptions has been moved from 0x" + tohex(sCursorOptionsOld) + " to 0x" + tohex(sCursorOptionsNew)
.endif