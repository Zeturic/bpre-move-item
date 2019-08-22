#pragma once
#include "task.h"
#include "main.h"

#define MENU_GIVE       0x04
#define MENU_TAKE_ITEM  0x05
#define MENU_MOVE_ITEM  0x12
#define MENU_CANCEL2    0x09

struct packed Struct203B0A0
{
    MainCallback exitCallback;
    TaskFunc unk4;
    u8 unk8_0:4;
    u8 mode:2;
    u8 unk8_2:2;
    s8 slotId;
    s8 unkA;
    u8 unkB;
    u16 unkC;
    s16 unkE;
    s16 unk10;
};

struct packed Struct203B0B4
{
    const struct Unk_Rodata1 *unk0;
    const u8 *unk4;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
    u8 fillerD[3];
};

struct packed Struct203B09C
{
    TaskFunc task;
    MainCallback exitCallback;
    u32 unk8_0:1;
    u32 unk8_1:3;
    u32 unk8_2:7;
    u32 unk9_0:7;
    u32 messageId:14;
    u8 windowId[3];
    u8 actions[8];
    u8 listSize;
    u16 palBuffer[0x100];
    s16 data[16];
};

extern struct Struct203B0A0 gUnknown_0203B0A0;
extern struct Struct203B0B4* gUnknown_0203B0B4;
extern struct Struct203B09C* gUnknown_0203B09C;

void sub_811F818(u8 slot, u8 b);
void sub_811FD88(u8 taskId, s8 *ptr);
u16 PartyMenuButtonHandler(s8 *ptr);
u8* GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 sub_81202F8(const u8* str, u8 b);
void display_pokemon_menu_message(u32 stringID);
void sub_8121CE4(u8 *ptr);
void sub_81224B4(struct Pokemon *mon, struct Struct203B0B4 *ptr);
void sub_8123BF0(u8 taskId);