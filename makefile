#!/usr/bin/env make
include $(DEVKITARM)/gba_rules

# ------------------------------------------------------------------------------

ROM_GBA = rom.gba
TEST_GBA = test.gba
TEST_SYM = test.sym

MAIN_ASM = main.asm
STRINGS_ASM = strings.asm
CHARACTER_ENCODING_TBL = character-encoding.tbl

RELOCATABLE_O = obj/relocatable.o

SRC_FILES = src/party_menu.c
OBJ_FILES = $(SRC_FILES:src/%.c=obj/%.o)
ASM_HEADERS = $(wildcard *.s)

CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CFLAGS = -O2 -mlong-calls -Wall -Wextra -Werror -mthumb -mno-thumb-interwork -fno-inline -fno-builtin -std=c11 -mcpu=arm7tdmi -march=armv4t -mtune=arm7tdmi -c -I include -D MSG_MOVE=$(MSG_MOVE)

LD = $(DEVKITARM)/bin/arm-none-eabi-ld
LDFLAGS = --relocatable

SIZE = $(DEVKITARM)/bin/arm-none-eabi-size
SIZEFLAGS = -d -B

ARMIPS ?= armips
ARMIPS_FLAGS = -sym $(TEST_SYM) -equ MSG_MOVE $(MSG_MOVE)

FREESIA = $(PYTHON) scripts/freesia.py
FREESIAFLAGS = --rom $(ROM_GBA) --start-at $(START_AT)

MSG_MOVE = 0x15
RESERVE = 100
START_AT ?= 0x08800000

# ------------------------------------------------------------------------------

.PHONY: all clean

all: $(RELOCATABLE_O) $(OBJ_FILES) $(TEST_GBA)

clean:
	rm -rf obj $(TEST_GBA) $(TEST_SYM)

obj/%.o:
	@mkdir -p obj
	$(CC) $(CFLAGS) $< -o $@

$(RELOCATABLE_O): $(OBJ_FILES)
	@mkdir -p obj
	$(LD) $(LDFLAGS) $? -o $@ 

$(TEST_GBA): $(ROM_GBA) $(MAIN_ASM) $(RELOCATABLE_O) $(ASM_HEADERS) $(STRINGS_ASM) $(CHARACTER_ENCODING_TBL)
	$(eval NEEDED_BYTES = $(shell expr $(shell $(SIZE) $(SIZEFLAGS) $(RELOCATABLE_O) |  awk 'FNR == 2 {print $$4}') + $(RESERVE)))
	$(ARMIPS) $(ARMIPS_FLAGS) $(MAIN_ASM) -definelabel allocation $(shell $(FREESIA) $(FREESIAFLAGS) --needed-bytes $(NEEDED_BYTES)) -equ allocation_size $(NEEDED_BYTES)

repoint-cursor-options:
	$(ARMIPS) repoint-cursor-options.asm

obj/%.d: src/%.c
	@mkdir -p obj
	@set -e; rm -f $@; \
	$(CC) -MT $(@:%.d=%.o) -M $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

include obj/party_menu.d