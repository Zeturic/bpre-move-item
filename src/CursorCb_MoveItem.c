#include "global.h"

void CursorCb_MoveItem(u8 taskid) {
    struct Task* task = &gTasks[taskid];

    PlaySE(SE_SELECT);

    sub_8121CE4(&gUnknown_0203B09C->field_C[1]);
    sub_8121CE4(&gUnknown_0203B09C->field_C[0]);

    struct Pokemon* pokemon = &gPlayerParty[brm.pokesel1];

    if (GetMonData(pokemon, MON_DATA_HELD_ITEM) != ITEM_NONE) {
        brm.field_B = 8;

        display_pokemon_menu_message(MSG_MOVE);
        sub_811F818(brm.pokesel1, 1);
        brm.pokesel2 = brm.pokesel1;
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
            if (button_unknown == 8 && gUnknown_0203B09C->unk8_0) {
                PlaySE(SE_SELECT);
                sub_8124258();
            }
        } else if (button_unknown == A_BUTTON) {
            PlaySE(SE_SELECT);

            u16 item1 = GetMonData(&gPlayerParty[brm.pokesel1], MON_DATA_HELD_ITEM);
            u16 item2 = GetMonData(&gPlayerParty[brm.pokesel2], MON_DATA_HELD_ITEM);

            SetMonData(&gPlayerParty[brm.pokesel1], MON_DATA_HELD_ITEM, &item2);
            SetMonData(&gPlayerParty[brm.pokesel2], MON_DATA_HELD_ITEM, &item1);

            sub_81224B4(
                &gPlayerParty[brm.pokesel1],
                &party_menu_something[brm.pokesel1]
            );

            sub_81224B4(
                &gPlayerParty[brm.pokesel2],
                &party_menu_something[brm.pokesel2]
            );

            sub_811FD88(taskid, pokesel);
        }
    }
}