MENU_GIVE       equ 0x04
MENU_TAKE_ITEM  equ 0x05
MENU_MOVE       equ 0x12
MENU_CANCEL2    equ 0x09

ACTION_ITEM_EXPANDED    equ 13

.definelabel gUnknown_0845A148, 0x0845A148
.definelabel sListSizeTable, 0x0845A760
.definelabel sActionTable, 0x0845A72C
.definelabel sCursorOptions, readu32("rom.gba", 0x08120F74 & 0x1FFFFFF)
.definelabel sActionStringTable, 0x0845A2E8

.definelabel gUnknown_0203B09C, 0x0203B09C
.definelabel gUnknown_0203B0A0, 0x0203B0A0
.definelabel gTasks, 0x03005090
.definelabel gPaletteFade, 0x02037AB8
.definelabel gUnknown_0203B0B4, 0x0203B0B4

.definelabel gUnknown_0845A120, 0x0845A120
.definelabel gUnknown_0845A128, 0x0845A128
.definelabel gUnknown_0845A130, 0x0845A130
.definelabel gUnknown_0845A138, 0x0845A138
.definelabel gUnknown_0845A140, 0x0845A140

.definelabel gPlayerParty, 0x02024284

.definelabel gStringVar1, 0x02021CD0
.definelabel gStringVar4, 0x02021D18

.definelabel gText_PkmnNotHolding, 0x08416C8F
.definelabel gText_PkmnWasGivenItem, 0x08416C2A

.definelabel gUnknown_0845A150, 0x0845A150
.definelabel gUnknown_0845A158, 0x0845A158
.definelabel sFontColorTable, 0x08459FFC