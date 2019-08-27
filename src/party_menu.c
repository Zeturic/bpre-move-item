#include "global.h"
#include "item.h"
#include "main.h"
#include "menu.h"
#include "menu_helpers.h"
#include "palette.h"
#include "party_menu.h"
#include "pokemon.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "constants/items.h"
#include "constants/songs.h"

void CursorCb_MoveItemCallback(u8 taskId)
{
    u16 item1, item2;
    u8 buffer[100];

    if (gPaletteFade.active || some_other_kind_of_link_test())
        return;

    switch (PartyMenuButtonHandler(&gUnknown_0203B0A0.unkA))
    {
    case 2:     // User hit B or A while on Cancel
        sub_811FD88(taskId, &gUnknown_0203B0A0.unkA);
        break;
    case 1:     // User hit A on a Pokemon
        // Pokemon can't give away items to eggs or themselves
        if (GetMonData(&gPlayerParty[gUnknown_0203B0A0.unkA], MON_DATA_IS_EGG)
            || gUnknown_0203B0A0.slotId == gUnknown_0203B0A0.unkA)
        {
            PlaySE(SE_I);
            return;
        }

        PlaySE(SE_SELECT);
        gUnknown_0203B0A0.unkB = 0;

        // look up held items
        item1 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM);
        item2 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.unkA], MON_DATA_HELD_ITEM);

        // swap the held items
        SetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM, &item2);
        SetMonData(&gPlayerParty[gUnknown_0203B0A0.unkA], MON_DATA_HELD_ITEM, &item1);

        // update the held item icons
        sub_81224B4(
            &gPlayerParty[gUnknown_0203B0A0.slotId],
            &gUnknown_0203B0B4[gUnknown_0203B0A0.slotId]
        );

        sub_81224B4(
            &gPlayerParty[gUnknown_0203B0A0.unkA],
            &gUnknown_0203B0B4[gUnknown_0203B0A0.unkA]
        );

        // create the string describing the move
        if (item2 == ITEM_NONE)
        {
            GetMonNickname(&gPlayerParty[gUnknown_0203B0A0.unkA], gStringVar1);
            CopyItemName(item1, gStringVar2);
            StringExpandPlaceholders(gStringVar4, gText_PkmnWasGivenItem);
        }
        else
        {
            GetMonNickname(&gPlayerParty[gUnknown_0203B0A0.slotId], gStringVar1);
            CopyItemName(item1, gStringVar2);
            StringExpandPlaceholders(buffer, gText_XsYAnd);

            StringAppend(buffer, gText_XsYWereSwapped);
            GetMonNickname(&gPlayerParty[gUnknown_0203B0A0.unkA], gStringVar1);
            CopyItemName(item2, gStringVar2);
            StringExpandPlaceholders(gStringVar4, buffer);
        }

        // display the string
        sub_81202F8(gStringVar4, 1);

        // update colors of selected boxes
        sub_811F818(gUnknown_0203B0A0.unkA, 0);
        sub_811F818(gUnknown_0203B0A0.slotId, 1);

        // return to the main party menu
        schedule_bg_copy_tilemap_to_vram(2);
        gTasks[taskId].func = sub_8123BF0;
        break;
    }
}

void CursorCb_MoveItem(u8 taskId)
{
    struct Pokemon *mon = &gPlayerParty[gUnknown_0203B0A0.slotId];

    PlaySE(SE_SELECT);

    // delete old windows
    sub_8121CE4(&gUnknown_0203B09C->windowId[1]);
    sub_8121CE4(&gUnknown_0203B09C->windowId[0]);

    if (GetMonData(mon, MON_DATA_HELD_ITEM) != ITEM_NONE)
    {
        gUnknown_0203B0A0.unkB = 8;

        // show "Move item to where" in bottom left
        display_pokemon_menu_message(MSG_MOVE);
        // update color of first selected box
        sub_811F818(gUnknown_0203B0A0.slotId, 1);

        // set up callback
        gUnknown_0203B0A0.unkA = gUnknown_0203B0A0.slotId;
        gTasks[taskId].func = CursorCb_MoveItemCallback;
    }
    else
    {
        // create and display string about lack of hold item
        GetMonNickname(mon, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
        sub_81202F8(gStringVar4, 1);

        // return to the main party menu
        schedule_bg_copy_tilemap_to_vram(2);
        gTasks[taskId].func = sub_8123BF0;
    }
}

const u8 gUnknown_ItemMenuActions[] = {MENU_GIVE, MENU_TAKE_ITEM, MENU_MOVE_ITEM, MENU_CANCEL2};
