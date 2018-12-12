#pragma once
#include "types.h"

#include "defines/attributes.h"

struct packed task {
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

struct packed pokemon_base {
    u8 _[80];
};

struct packed pokemon {
    struct pokemon_base base;
    u8 _[20];
};

struct packed brmo2 {
    u8 field_0;
    u8 field_1;
    u8 field_2;
    u8 field_3;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 field_9;
    u8 field_A;
    u8 field_B;
    u8 field_C;
    u8 field_D;
    u8 field_E;
    u8 seq[8];
    u8 cur;
    u8 unknown[540];
    u16 field_234;
    u16 field_236;
    u8 field_238;
};

struct packed brmo {
    u32 func_c2;
    u8 field_4;
    u8 field_5;
    u8 field_6;
    u8 field_7;
    u8 field_8;
    u8 pokesel1;
    u8 pokesel2;
    u8 field_B;
    u8 field_C;
    u8 field_D;
    u16 field_E;
};

struct fade_ctrl {
    u16 bg_pal_bitfield;

    u16 obj_pal_bitfield;

    u16 delay_counter : 6;
    u16 y : 5; // blend coefficient
    u16 target_y : 5; // target blend coefficient

    u16 blend_color : 15;
    u16 active : 1;

    u16 multipurpose2 : 6;
    u16 y_dec : 1; // whether blend coefficient is decreasing
    u16 buffer_transfer_disabled : 1;
    u16 mode : 2;
    u16 should_reset_blend_registers : 1;
    u16 hardware_fade_finishing : 1;

    u16 software_fade_finishing_counter : 5;
    u16 software_fade_finishing : 1;
    u16 obj_palette_toggle : 1;
    u16 _ : 1;
    u16 delta_y : 4; // rate of change of blend coefficient
};