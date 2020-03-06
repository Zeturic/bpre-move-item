EXTRA_CFLAGS = -D PARTY_MSG_MOVE_ITEM_WHERE=$(PARTY_MSG_MOVE_ITEM_WHERE) -D MENU_MOVE_ITEM=$(MENU_MOVE_ITEM)

EXTRA_ARMIPS_FLAGS = -equ PARTY_MSG_MOVE_ITEM_WHERE $(PARTY_MSG_MOVE_ITEM_WHERE) -equ MENU_MOVE_ITEM $(MENU_MOVE_ITEM)

PARTY_MSG_MOVE_ITEM_WHERE = 0x15

# ------------------------------------------------------------------------------

.PHONY: repoint-cursor-options

repoint-cursor-options:
	$(ARMIPS) repoint-cursor-options.asm
