#include "global.h"

void brm_move(u8 taskid) {
    struct task* task = &tasks[taskid];
    // struct pokemon* pokemon = &party_player[brm.pokesel1];

    audio_play(AUDIO_GENERICCLINK);
    brm.field_B = 8;

    sub_8121CE4(&ptr_brmo2->field_D);
    sub_8121CE4(&ptr_brmo2->field_C);

    choosePokemonStrings(0x15);
    sub_811F818(brm.pokesel1, 1);
    brm.pokesel2 = brm.pokesel1;
    task->fn = callback;
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
            if (button_unknown == 8 && ptr_brmo2->field_8) {
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