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
    u8 filler_0[100];
};

struct packed Struct203B09C {
    void (*task)();
    void (*exitCallback)();

    u32 field_8_0 : 1;
    u32 field_8_1 : 3;
    u32 field_8_2 : 7;
    u32 field_8_3 : 7;
    u32 messageId : 14;

    u8 windowId[3];
    u8 actions[8];
    u8 listSize;

    u16 pal_buffer[0x100];
    s16 private[16];
};

struct packed Struct203B0A0 {
    void (*callback)();
    void (*task)();

    u8 field_8_0:4;
    u8 mode:2;
    u8 field_8_2:2;

    s8 slotId;
    s8 pokesel2;

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

struct packed Unk_Rodata1 {
    void (*fn)(u8, u8, u8, u8, u8, u8);
    u8 field_4[24];
    u8 field_1C;
    u8 field_1D;
    u8 field_1E;
    u8 field_1F;
};

struct packed Struct203B0B4 {
    struct Unk_Rodata1* rodata;
    u8* field_4;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
    u8 filler_D[3];
};