#!/usr/bin/env make

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_tools
include config.mk
include project.mk

# ------------------------------------------------------------------------------

SRC_FILES ?= $(wildcard src/*.c)
OBJ_FILES ?= $(SRC_FILES:src/%.c=build/src/%.o)
MAIN_ASM_INCLUDES ?= $(wildcard *.s)

CPPFLAGS ?= -I include -I gflib

CFLAGS = -O2 -mlong-calls -Wall -Wextra -mthumb -mno-thumb-interwork -fno-inline -fno-builtin -std=gnu11 -mabi=apcs-gnu -mcpu=arm7tdmi -march=armv4t -mtune=arm7tdmi -x c -c -MMD $(CPPFLAGS) $(EXTRA_CFLAGS)

LD = $(PREFIX)ld
LDFLAGS = --relocatable -T rom.ld $(EXTRA_LDFLAGS)

SIZE = $(PREFIX)size
SIZEFLAGS = -d -B

PRET_PREPROC ?= pret-preproc

ARMIPS ?= armips
ARMIPS_FLAGS = -sym test.sym $(EXTRA_ARMIPS_FLAGS)

PYTHON ?= python3
FREESIA = $(PYTHON) tools/freesia
FREESIAFLAGS = --rom rom.gba --start-at $(START_AT)

# ------------------------------------------------------------------------------

.DEFAULT_GOAL = all

.PHONY: all clean md5

all: test.gba

clean:
	rm -rf build test.gba test.sym $(CLEANABLE_FILES)

md5: test.gba
	@md5sum test.gba

# ------------------------------------------------------------------------------

build/linked.sz: build/linked.o
	echo "`$(SIZE) $(SIZEFLAGS) build/linked.o | awk 'FNR == 2 {print $$4}'` + 8" | bc > "$@"

build/src/%.o: src/%.c charmap.txt
	@mkdir -p build/src
	(echo '#line 1 "$<"' && $(PRET_PREPROC) "$<" charmap.txt) | $(CC) $(CFLAGS) -MF "$(@:%.o=%.d)" -MT "$@" -o "$@" -

build/linked.o: $(OBJ_FILES) rom.ld
	@mkdir -p build
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o "$@"

test.gba: rom.gba main.asm build/linked.o build/linked.sz $(MAIN_ASM_INCLUDES)
	$(ARMIPS) $(ARMIPS_FLAGS) main.asm -definelabel allocation `$(FREESIA) $(FREESIAFLAGS) --needed-bytes $$(cat build/linked.sz)` -equ allocation_size `cat build/linked.sz`

-include $(SRC_FILES:src/%.c=build/src/%.d)
