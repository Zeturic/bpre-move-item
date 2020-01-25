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

struct packed PartyMenuBox
{
    const struct PartyMenuBoxInfoRects *infoRects;
    const u8 *spriteCoords;
    u8 windowId;
    u8 monSpriteId;
    u8 itemSpriteId;
    u8 pokeballSpriteId;
    u8 statusSpriteId;
    u8 fillerD[3];
};

struct packed PartyMenuInternal
{
    TaskFunc task;
    MainCallback exitCallback;
    u32 chooseHalf:1;
    u32 lastSelectedSlot:3;
    u32 spriteIdConfirmPokeball:7;
    u32 spriteIdCancelPokeball:7;
    u32 messageId:14;
    u8 windowId[3];
    u8 actions[8];
    u8 numActions;
    u16 palBuffer[BG_PLTT_SIZE / sizeof(u16)];
    s16 data[16];
};

extern struct PartyMenu gPartyMenu;
extern struct PartyMenuBox* sPartyMenuBoxes;
extern struct PartyMenuInternal* sPartyMenuInternal;

void AnimatePartySlot(u8 slot, u8 animNum);
void HandleChooseMonCancel(u8 taskId, s8 *ptr);
u16 PartyMenuButtonHandler(s8 *ptr);
u8* GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 DisplayPartyMenuMessage(const u8* str, bool8 keepOpen);
void DisplayPartyMenuStdMessage(u32 stringID);
void PartyMenuRemoveWindow(u8 *ptr);
void UpdatePartyMonHeldItemSprite(struct Pokemon *mon, struct PartyMenuBox *ptr);
void Task_UpdateHeldItemSprite(u8 taskId);
