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

    .align 2
    // expanded ITEM menu window template
    gUnknown_CustomWindowTemplate:
        window_template 0x02, 0x16, 0x0B, 0x07, 0x08, 0x0E, 0x373
    .thumb
.endarea

.org sCursorOptions + MENU_MOVE * 8
.word gText_Move, CursorCb_MoveItem |1

.org sActionStringTable + MSG_MOVE * 4
.word gText_MoveWhere

.org 0x081236A0     // CursorCb_Item
.area 0x64, 0xFE
    ldr r3, =CursorCb_Item |1
    bx r3
    .pool
.endarea

.org 0x08121E5C     // sub_8121E5C
.area 0x164, 0xFE
    ldr r3, =sub_8121E5C |1
    bx r3
    .pool
.endarea

.close