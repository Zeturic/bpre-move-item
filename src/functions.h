#pragma once
#include "types.h"

void audio_play(u8 audio_id);
void sub_8121CE4(u8* unk);
void choosePokemonStrings(u32 id);
void sub_811F818(u8 partyid, u8 unk);
bool some_other_kind_of_link_test(u32 unk);
u8* get_pokesel();
u8 sub_811FEFC(u8* pokesel);
void sub_811FD88(u8 taskid, u8* pokesel);
void sub_811FBC4(u8 taskid, u8* pokesel);
void sub_8124258();

void callback(u8 taskid);