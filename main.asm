.gba
.include "macros.s"

.arm
.include "locations.s"

.thumb
.include "functions.s"

.loadtable "character-encoding.tbl"

.open "rom.gba", "test.gba", 0x08000000

.org allocation
.area allocation_size
    .importobj "obj/relocatable.o"

    .arm
    .include "strings.asm"
    .thumb

    item_brm_strings:
        .byte MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE, MENU_CANCEL2
.endarea

// item brm window template
.org gUnknown_0845A148
window_template 0x02, 0x16, 0x0B, 0x07, 0x08, 0x0E, 0x373

// item brm num entries
.org sListSizeTable + 8
.byte 4

// item brm strings
.org sActionTable + 8 * 4
.word item_brm_strings

.org sCursorOptions + MENU_MOVE * 8
.word gText_Move, CursorCb_MoveItem |1

.org sActionStringTable + MSG_MOVE * 4
.word gText_MoveWhere

.close