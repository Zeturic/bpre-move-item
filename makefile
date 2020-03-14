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

HEADER_DIRS ?= -I include -I gflib

CFLAGS = -O2 -mlong-calls -Wall -Wextra -mthumb -mno-thumb-interwork -fno-inline -fno-builtin -std=gnu11 -mabi=apcs-gnu -mcpu=arm7tdmi -march=armv4t -mtune=arm7tdmi -x c -c $(HEADER_DIRS) $(EXTRA_CFLAGS)

LD = $(PREFIX)ld
LDFLAGS = --relocatable -T rom.ld $(EXTRA_LDFLAGS)

SIZE = $(PREFIX)size
SIZEFLAGS = -d -B

PREPROC = tools/preproc/preproc
SCANINC = tools/scaninc/scaninc

ARMIPS ?= armips
ARMIPS_FLAGS = -sym test.sym $(EXTRA_ARMIPS_FLAGS)

PYTHON ?= python
FREESIA = $(PYTHON) tools/freesia
FREESIAFLAGS = --rom rom.gba --start-at $(START_AT)

# ------------------------------------------------------------------------------

.PHONY: all spotless clean clean-tools md5

all: test.gba

spotless: clean clean-tools

clean:
	rm -rf build test.gba test.sym

clean-tools:
	+BUILD_TOOLS_TARGET=clean ./build_tools.sh

md5: test.gba
	@md5sum test.gba

# ------------------------------------------------------------------------------

build/linked.sz: build/linked.o
	$(SIZE) $(SIZEFLAGS) build/linked.o | awk 'FNR == 2 {print $$4}' > "$@"

build/linked.alloc: rom.gba build/linked.sz
	$(FREESIA) $(FREESIAFLAGS) --needed-bytes $(shell cat build/linked.sz) > "$@"

build/src/%.o: src/%.c charmap.txt
	@mkdir -p build/src
	(echo '#line 1 "$<"' && $(PREPROC) "$<" charmap.txt) | $(CC) $(CFLAGS) -o "$@" -

build/linked.o: $(OBJ_FILES) rom.ld
	@mkdir -p build
	$(LD) $(LDFLAGS) $(OBJ_FILES) -o "$@"

test.gba: rom.gba main.asm build/linked.o build/linked.sz build/linked.alloc $(MAIN_ASM_INCLUDES)
	$(ARMIPS) $(ARMIPS_FLAGS) main.asm -definelabel allocation $(shell cat build/linked.alloc) -equ allocation_size $(shell cat build/linked.sz)

build/dep/src/%.d: src/%.c
	@mkdir -p build/dep/src
	@$(SCANINC) $(HEADER_DIRS) $< | awk '{print "$(<:src/%.c=build/src/%.o) $@ : "$$0}' > "$@"

include $(SRC_FILES:src/%.c=build/dep/src/%.d)
