.gba
.include "macros.s"
.include "constants.s"

.arm
.include "locations.s"

.thumb

.loadtable "character-encoding.tbl"

.open "rom.gba", "test.gba", 0x08000000

.org allocation
.area allocation_size
    .importobj "obj/relocatable.o"
.endarea

// expanded ITEM menu window template
.org gUnknown_0845A148
    window_template 0x02, 0x16, 0x0B, 0x07, 0x08, 0x0E, 0x373

.org sListSizeTable + ACTION_ITEM * SIZEOF_U8
.byte 4

.org sActionTable + ACTION_ITEM * SIZEOF_PTR
.word gUnknown_ItemMenuActions

.org sCursorOptions + MENU_MOVE_ITEM * SIZEOF_CURSOR_OPTION
.word gMenuText_Move, CursorCb_MoveItem |1

.org sActionStringTable + MSG_MOVE * 4
.word gText_MoveItemWhere

.close