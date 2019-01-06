#include "global.h"

void brm_move(u8 taskid) {
    struct task* task = &tasks[taskid];

    audio_play(AUDIO_GENERICCLINK);

    sub_8121CE4(&ptr_brmo2->field_C[1]);
    sub_8121CE4(&ptr_brmo2->field_C[0]);

    struct pokemon* pokemon = &party_player[brm.pokesel1];

    if (pokemon_getattr(pokemon, REQ_HELDITEM) != ITEM_NONE) {
        brm.field_B = 8;

        choosePokemonStrings(MSG_MOVE);
        sub_811F818(brm.pokesel1, 1);
        brm.pokesel2 = brm.pokesel1;
        task->fn = callback;

    } else {
        pokemon_getnick(pokemon, fcode_buffer2);
        fdecoder(fcode_buffer5, pXIsntHoldingAnything);
        sub_81202F8(fcode_buffer5, 1);

        bgid_mark_for_sync(2);
        task->fn = sub_8123BF0;
    }
}

void callback(u8 taskid) {
    if (! (pal_fade_control.active || some_other_kind_of_link_test(0))) {
        u8* pokesel = get_pokesel();
        u8 button_unknown = sub_811FEFC(pokesel);

        if (button_unknown == BTTN_B) {
            // also for explicitly selecting cancel
            sub_811FD88(taskid, pokesel);
        } else if (2 < button_unknown) {
            // no idea
            if (button_unknown == 8 && ptr_brmo2->unk8_0) {
                audio_play(AUDIO_GENERICCLINK);
                sub_8124258();
            }
        } else if (button_unknown == BTTN_A) {
            // sub_811FBC4(taskid, pokesel);
            audio_play(AUDIO_GENERICCLINK);
            sub_8122E5C(taskid);
        }
    }
}