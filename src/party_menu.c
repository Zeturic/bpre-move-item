#include "global.h"
#include "item.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.c.h"
#include "pokemon.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "constants/items.h"
#include "constants/party_menu.h"
#include "constants/songs.h"

void CursorCb_MoveItemCallback(u8 taskId)
{
    u16 item1, item2;
    u8 buffer[100];

    if (gPaletteFade.active || some_other_kind_of_link_test())
        return;

    switch (PartyMenuButtonHandler(&gPartyMenu.slotId2))
    {
    case 2:     // User hit B or A while on Cancel
        HandleChooseMonCancel(taskId, &gPartyMenu.slotId2);
        break;
    case 1:     // User hit A on a Pokemon
        // Pokemon can't give away items to eggs or themselves
        if (GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_IS_EGG)
            || gPartyMenu.slotId == gPartyMenu.slotId2)
        {
            PlaySE(SE_HAZURE);
            return;
        }

        // Credit to Deokishisu
        if(GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM) >= ITEM_ORANGE_MAIL
        && GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM) <= ITEM_RETRO_MAIL)
        {
            PlaySE(SE_HAZURE);
            return;
        }

        PlaySE(SE_SELECT);
        gPartyMenu.action = PARTY_ACTION_CHOOSE_MON;

        // look up held items
        item1 = GetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM);
        item2 = GetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM);

        // swap the held items
        SetMonData(&gPlayerParty[gPartyMenu.slotId], MON_DATA_HELD_ITEM, &item2);
        SetMonData(&gPlayerParty[gPartyMenu.slotId2], MON_DATA_HELD_ITEM, &item1);

        // update the held item icons
        UpdatePartyMonHeldItemSprite(
            &gPlayerParty[gPartyMenu.slotId],
            &sPartyMenuBoxes[gPartyMenu.slotId]
        );

        UpdatePartyMonHeldItemSprite(
            &gPlayerParty[gPartyMenu.slotId2],
            &sPartyMenuBoxes[gPartyMenu.slotId2]
        );

        // create the string describing the move
        if (item2 == ITEM_NONE)
        {
            GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
            CopyItemName(item1, gStringVar2);
            StringExpandPlaceholders(gStringVar4, gText_PkmnWasGivenItem);
        }
        else
        {
            GetMonNickname(&gPlayerParty[gPartyMenu.slotId], gStringVar1);
            CopyItemName(item1, gStringVar2);
            StringExpandPlaceholders(buffer, gText_XsYAnd);

            StringAppend(buffer, gText_XsYWereSwapped);
            GetMonNickname(&gPlayerParty[gPartyMenu.slotId2], gStringVar1);
            CopyItemName(item2, gStringVar2);
            StringExpandPlaceholders(gStringVar4, buffer);
        }

        // display the string
        DisplayPartyMenuMessage(gStringVar4, TRUE);

        // update colors of selected boxes
        AnimatePartySlot(gPartyMenu.slotId2, 0);
        AnimatePartySlot(gPartyMenu.slotId, 1);

        // return to the main party menu
        schedule_bg_copy_tilemap_to_vram(2);
        gTasks[taskId].func = Task_UpdateHeldItemSprite;
        break;
    }
}

void CursorCb_MoveItem(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gPartyMenu.slotId];

    PlaySE(SE_SELECT);

    // delete old windows
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[1]);
    PartyMenuRemoveWindow(&sPartyMenuInternal->windowId[0]);

    if (GetMonData(mon, MON_DATA_HELD_ITEM) != ITEM_NONE)
    {
        gPartyMenu.action = PARTY_ACTION_SWITCH;

        // show "Move item to where" in bottom left
        DisplayPartyMenuStdMessage(PARTY_MSG_MOVE_ITEM_WHERE);
        // update color of first selected box
        AnimatePartySlot(gPartyMenu.slotId, 1);

        // set up callback
        gPartyMenu.slotId2 = gPartyMenu.slotId;
        gTasks[taskId].func = CursorCb_MoveItemCallback;
    }
    else
    {
        // create and display string about lack of hold item
        GetMonNickname(mon, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
        DisplayPartyMenuMessage(gStringVar4, TRUE);

        // return to the main party menu
        schedule_bg_copy_tilemap_to_vram(2);
        gTasks[taskId].func = Task_UpdateHeldItemSprite;
    }
}

const u8 sPartyMenuAction_GiveTakeItemCancel[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};
