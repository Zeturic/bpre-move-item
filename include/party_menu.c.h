#pragma once

#include "main.h"
#include "party_menu.h"
#include "task.h"

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

extern struct PartyMenuBox* sPartyMenuBoxes;
extern struct PartyMenuInternal* sPartyMenuInternal;

void HandleChooseMonCancel(u8 taskId, s8 *ptr);
u16 PartyMenuButtonHandler(s8 *ptr);
void PartyMenuRemoveWindow(u8 *ptr);
void UpdatePartyMonHeldItemSprite(struct Pokemon *mon, struct PartyMenuBox *ptr);
void Task_UpdateHeldItemSprite(u8 taskId);
