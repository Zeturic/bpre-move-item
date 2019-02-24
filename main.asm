.gba

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
        .byte 0x04, 0x05, moveitemid, 0x09
.endarea

// item brm rboxid
.org stru_845A120 + 5 * 8
.byte 0x02, 0x16, 0x0B, 0x07, 0x08, 0x0E, 0x73, 0x03

// item brm num entries
.org unk_845A760 + 8
.byte 4

// item brm strings
.org off_845A72C + 8 * 4
.word item_brm_strings

.org bottom_right_menu + moveitemid * 8
.word gText_Move, CursorCb_MoveItem |1

.org msg_mix + MSG_MOVE * 4
.word gText_MoveWhere

.close