#pragma once
#include "types.h"

#define packed __attribute__((packed))

struct packed Task {
    void (*fn)(u8 taskid);
    u8 id;
    u8 prev;
    u8 next;
    u8 priority;
    u16 private_00;
    u16 private_01;
    u16 private_02;
    u16 private_03;
    u16 private_04;
    u16 private_05;
    u16 private_06;
    u16 private_07;
    u16 private_08;
    u16 private_09;
    u16 private_10;
    u16 private_11;
    u16 private_12;
    u16 private_13;
    u16 private_14;
    u16 private_15;
};

struct packed Pokemon {
    u8 unk0[100];
};

struct packed Struct203B09C {
    void (*task)();
    void (*callback)();

    u32 unk8_0 : 1;
    u32 unk8_1 : 3;
    u32 unk8_2 : 7;
    u32 unk8_3 : 7;
    u32 unk8_4 : 14;

    u8 field_C[3];
    u8 seq[8];
    u8 cur;

    u16 pal_buffer[0xB0];
    u8 filler[0xA0];
    s16 private[16];
};

struct packed brmo {
    void (*callback)();
    void (*task)();

    u8 unk8_0:4;
    u8 mode:2;
    u8 unk8_2:2;

    u8 pokesel1;
    u8 pokesel2;
    u8 field_B;
    u16 item;
    u16 move;
    u16 field_10;
};

struct packed PaletteFadeControl {
    u32 multipurpose1;
    u8 delayCounter:6;
    u16 y:5; // blend coefficient
    u16 targetY:5; // target blend coefficient
    u16 blendColor:15;
    bool16 active:1;
    u16 multipurpose2:6;
    bool16 yDec:1; // whether blend coefficient is decreasing
    bool16 bufferTransferDisabled:1;
    u16 mode:2;
    bool16 shouldResetBlendRegisters:1;
    bool16 hardwareFadeFinishing:1;
    u16 softwareFadeFinishingCounter:5;
    bool16 softwareFadeFinishing:1;
    bool16 objPaletteToggle:1;
    u8 deltaY:4; // rate of change of blend coefficient
    u8 unkB;
};

struct rodata {
    void (*fn)(u8, u8, u8, u8, u8, u8);
    u8 field_4[24];
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
};

struct packed party_menu_something {
    struct rodata* rodata;
    u8* field_4;
    u8 windowid;
    u8 party_icon;
    u8 held_item;
    u8 poke_ball;
    u8 status;
    u8 filler[3];
};