#include "global.h"

void mvitem(u8 taskid) {
    struct task* task = &tasks[taskid];
    // struct pokemon* pokemon = &party_player[brm.pokesel1];

    audio_play(AUDIO_GENERICCLINK);
    brm.field_B = 8;

    sub_8121CE4(&ptr_brmo2->field_D);
    sub_8121CE4(&ptr_brmo2->field_C);

    choosePokemonStrings(3);
    sub_811F818(brm.pokesel1, 1);
    brm.pokesel2 = brm.pokesel1;
    task->fn = 0x0811FB28 |1;
}