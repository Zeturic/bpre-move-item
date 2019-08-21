#!/usr/bin/env make
include config.mk
include $(DEVKITARM)/gba_rules

GCC_DEFINES = -D ACTION_STR_MOVE=$(ACTION_STR_MOVE)
ARMIPS_DEFINES = -equ ACTION_STR_MOVE $(ACTION_STR_MOVE)

# ------------------------------------------------------------------------------

OBJ = obj
SRC = src
INCLUDE = include

OPTIMIZATION_LEVEL ?= -O2
RESERVE ?= 0
START_AT ?= 0x08800000

C_HEADERS = $(wildcard src/*.h) $(wildcard src/constants/*.h) config.mk
ASM_HEADERS = $(wildcard *.s) character-encoding.tbl config.mk strings.asm

ROM_GBA ?= rom.gba
TEST_GBA ?= test.gba
TEST_SYM = $(TEST_GBA:%.gba=%.sym)

GCC = $(DEVKITARM)/bin/arm-none-eabi-gcc
GCC_DEFINES ?=
GCC_FLAGS = -Wall -Wextra -Werror -mthumb -mno-thumb-interwork -fno-inline -fno-builtin -std=c11 -mcpu=arm7tdmi -march=armv4t -mtune=arm7tdmi -c $(OPTIMIZATION_LEVEL) $(GCC_DEFINES) -I $(INCLUDE)

LD = $(DEVKITARM)/bin/arm-none-eabi-ld
LD_FLAGS = --relocatable

SIZE = $(DEVKITARM)/bin/arm-none-eabi-size
SIZE_FLAGS = -d -B

ARMIPS ?= armips
ARMIPS_DEFINES ?=
ARMIPS_FLAGS = -sym $(TEST_SYM) $(ARMIPS_DEFINES) -strequ ROM_GBA $(ROM_GBA) -strequ TEST_GBA $(TEST_GBA)

PYTHON ?= python
FREESIA = $(PYTHON) scripts/freesia.py

MAIN_ASM = main.asm
RELOCATABLE_O = $(OBJ)/relocatable.o

RELOCATABLE_OBJECTS ?= $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(wildcard src/*.c))

# ------------------------------------------------------------------------------

default: $(TEST_GBA)

clean:
	rm -rf $(OBJ) $(TEST_GBA) $(TEST_SYM)

$(TEST_GBA): $(ROM_GBA) $(MAIN_ASM) $(ASM_HEADERS) $(RELOCATABLE_O)
	$(eval NEEDED_BYTES = $(shell expr $(shell $(SIZE) $(SIZE_FLAGS) $(RELOCATABLE_O) |  awk 'FNR == 2 {print $$4}') + $(RESERVE)))
	$(ARMIPS) $(ARMIPS_FLAGS) $(MAIN_ASM) -definelabel allocation $(shell $(FREESIA) --rom $(ROM_GBA) --needed-bytes $(NEEDED_BYTES) --start-at $(START_AT)) -equ allocation_size $(NEEDED_BYTES)

$(RELOCATABLE_O): $(RELOCATABLE_OBJECTS)
	mkdir -p $(OBJ)
	$(LD) $(LD_FLAGS) -o $@ $?

$(OBJ)/%.o: $(SRC)/%.c $(C_HEADERS)
	mkdir -p $(OBJ)
	$(GCC) $(GCC_FLAGS) -mlong-calls $< -o $@

repoint-cursor-options:
	$(ARMIPS) repoint-cursor-options.asm

repoint-action-tables:
	$(ARMIPS) repoint-action-tables.asm