#!/usr/bin/env make

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools
include config.mk

# ------------------------------------------------------------------------------

SRC_FILES = src/party_menu.c src/strings.c
OBJ_FILES = $(SRC_FILES:src/%.c=obj/%.o)
ASM_HEADERS = $(wildcard *.s)

CFLAGS = -O2 -mlong-calls -Wall -Wextra -Werror -mthumb -mno-thumb-interwork -fno-inline -fno-builtin -std=c11 -mcpu=arm7tdmi -march=armv4t -mtune=arm7tdmi -x c -c -I include -D MSG_MOVE=$(MSG_MOVE) -D MENU_MOVE_ITEM=$(MENU_MOVE_ITEM)

LD = $(PREFIX)ld
LDFLAGS = --relocatable -T rom.ld

SIZE = $(PREFIX)size
SIZEFLAGS = -d -B

PREPROC = tools/preproc/preproc

ARMIPS ?= armips
ARMIPS_FLAGS = -sym test.sym -equ MSG_MOVE $(MSG_MOVE) -equ MENU_MOVE_ITEM $(MENU_MOVE_ITEM)

PYTHON ?= python
FREESIA = $(PYTHON) tools/freesia
FREESIAFLAGS = --rom rom.gba --start-at $(START_AT)

MSG_MOVE = 0x15
START_AT ?= 0x0871A240

# ------------------------------------------------------------------------------

.PHONY: all clean repoint-cursor-options

all: test.gba

clean:
	rm -rf obj test.gba test.sym

obj/%.o:
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

obj/strings.o: src/strings.c charmap.txt
	@mkdir -p obj
	$(PREPROC) $^ | $(CC) $(CFLAGS) -o $@ -

obj/relocatable.o: $(OBJ_FILES) rom.ld
	@mkdir -p obj
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o $@

test.gba: rom.gba main.asm obj/relocatable.o $(ASM_HEADERS)
	$(eval NEEDED_BYTES = $(shell PATH="$(PATH)" $(SIZE) $(SIZEFLAGS) obj/relocatable.o |  awk 'FNR == 2 {print $$4}'))
	$(ARMIPS) $(ARMIPS_FLAGS) main.asm -definelabel allocation $(shell $(FREESIA) $(FREESIAFLAGS) --needed-bytes $(NEEDED_BYTES)) -equ allocation_size $(NEEDED_BYTES)

repoint-cursor-options:
	$(ARMIPS) repoint-cursor-options.asm

obj/%.d: src/%.c
	@mkdir -p obj
	@set -e; rm -f $@; \
	$(CC) -MT $(@:%.d=%.o) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include obj/party_menu.d