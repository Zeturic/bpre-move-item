.gba
.thumb

.include "macros.s"
.include "constants.s"

.open "rom.gba", "test.gba", 0x08000000

.org allocation
.area allocation_size
    .importobj "build/linked.o"
.endarea

// adjust size of the ITEM menu window template
.org sItemGiveTakeWindowTemplate + 1
.byte 22, 11, 7, 8

.org sPartyMenuActionCounts + ACTIONS_ITEM * SIZEOF_U8
.byte 4

.org sPartyMenuActions + ACTIONS_ITEM * SIZEOF_PTR
.word sPartyMenuAction_GiveTakeItemCancel

.org sCursorOptions + MENU_MOVE_ITEM * SIZEOF_CURSOR_OPTION
.word gMenuText_Move, CursorCb_MoveItem |1

.org sActionStringTable + PARTY_MSG_MOVE_ITEM_WHERE * 4
.word gText_MoveItemWhere

.close

.definelabel sCursorOptions, readu32("rom.gba", 0x08120F74 & 0x1FFFFFF)
