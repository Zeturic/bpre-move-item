.definelabel free_space, 0x08800800

NUM_ACTIONS_OLD equ 13
NUM_ADDITIONAL_ACTIONS equ 3

.definelabel sActionTableOld, 0x0845A72C
.definelabel sListSizeTableOld, 0x0845A760

// -----------------------------------------------------------------------------

NUM_ACTIONS_NEW equ (NUM_ACTIONS_OLD + NUM_ADDITIONAL_ACTIONS)

SIZEOF_PTR equ 4
SIZEOF_U8 equ 1

// -----------------------------------------------------------------------------

.include "macros.s"

.gba
.open "rom.gba", 0x08000000

// -----------------------------------------------------------------------------

// copy the contents of the tables to their new locations

.org free_space

.align 4
sActionTableNew:
    incbinm "rom.gba", sActionTableOld, NUM_ACTIONS_OLD * SIZEOF_PTR
    .fill NUM_ADDITIONAL_ACTIONS * SIZEOF_PTR, 0

.align 1
sListSizeTableNew:
    incbinm "rom.gba", sListSizeTableOld, NUM_ACTIONS_OLD * SIZEOF_U8
    .fill NUM_ADDITIONAL_ACTIONS * SIZEOF_U8, 0

// -----------------------------------------------------------------------------

// delete the data at the old locations

.org sActionTableOld
    .fill NUM_ACTIONS_OLD * SIZEOF_PTR, 0xFF

.org sListSizeTableOld
    .fill NUM_ACTIONS_OLD * SIZEOF_U8, 0xFF

// -----------------------------------------------------------------------------

safeupdateptr 0x081229F8, sActionTableOld, sActionTableNew
safeupdateptr 0x081229F4, sListSizeTableOld, sListSizeTableNew

// -----------------------------------------------------------------------------

.close

.notice "sActionTable has been moved from 0x" + tohex(sActionTableOld) + " to 0x" + tohex(sActionTableNew)
.notice "sListSizeTable has been moved from 0x" + tohex(sListSizeTableOld) + " to 0x" + tohex(sListSizeTableNew)