#include "global.h"

void CursorCb_MoveItem(u8 taskid) {
    struct Task* task = &gTasks[taskid];

    PlaySE(SE_SELECT);

    sub_8121CE4(&gUnknown_0203B09C->windowId[1]);
    sub_8121CE4(&gUnknown_0203B09C->windowId[0]);

    struct Pokemon* pokemon = &gPlayerParty[gUnknown_0203B0A0.slotId];

    if (GetMonData(pokemon, MON_DATA_HELD_ITEM) != ITEM_NONE) {
        gUnknown_0203B0A0.field_B = 8;

        display_pokemon_menu_message(MSG_MOVE);
        sub_811F818(gUnknown_0203B0A0.slotId, 1);
        gUnknown_0203B0A0.pokesel2 = gUnknown_0203B0A0.slotId;
        task->fn = callback;

    } else {
        GetMonNick(pokemon, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
        sub_81202F8(gStringVar4, 1);

        schedule_bg_copy_tilemap_to_vram(2);
        task->fn = sub_8123BF0;
    }
}

void callback(u8 taskid) {
    if (! (gPaletteFade.active || some_other_kind_of_link_test(0))) {
        s8* pokesel = get_pokesel();
        u8 button_unknown = PartyMenuButtonHandler(pokesel);

        if (button_unknown == B_BUTTON) {
            // also for explicitly selecting cancel
            sub_811FD88(taskid, pokesel);
        } else if (2 < button_unknown) {
            // no idea
            if (button_unknown == 8 && gUnknown_0203B09C->field_8_0) {
                PlaySE(SE_SELECT);
                sub_8124258();
            }
        } else if (button_unknown == A_BUTTON) {
            PlaySE(SE_SELECT);

            u16 item1 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM);
            u16 item2 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.pokesel2], MON_DATA_HELD_ITEM);

            SetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM, &item2);
            SetMonData(&gPlayerParty[gUnknown_0203B0A0.pokesel2], MON_DATA_HELD_ITEM, &item1);

            sub_81224B4(
                &gPlayerParty[gUnknown_0203B0A0.slotId],
                &gUnknown_0203B0B4[gUnknown_0203B0A0.slotId]
            );

            sub_81224B4(
                &gPlayerParty[gUnknown_0203B0A0.pokesel2],
                &gUnknown_0203B0B4[gUnknown_0203B0A0.pokesel2]
            );

            sub_811FD88(taskid, pokesel);
        }
    }
}