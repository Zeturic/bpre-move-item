#pragma once
#include "task.h"
#include "main.h"

#define MENU_GIVE       0x04
#define MENU_TAKE_ITEM  0x05
#define MENU_CANCEL2    0x09

struct packed PartyMenu
{
    MainCallback exitCallback;
    TaskFunc task;
    u8 menuType:4;
    u8 layout:2;
    u8 unk8_2:2;
    s8 slotId;
    s8 slotId2;
    u8 action;
    u16 bagItem;
    s16 data1;
    s16 learnMoveState;
};

extern struct PartyMenu gPartyMenu;

void AnimatePartySlot(u8 slot, u8 animNum);
u8* GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 DisplayPartyMenuMessage(const u8* str, bool8 keepOpen);
void DisplayPartyMenuStdMessage(u32 stringID);
