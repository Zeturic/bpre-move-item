#pragma once
#include "types.h"

void audio_play(u8 audio_id);
void sub_8121CE4(u8* unk);
void display_pokemon_menu_message(u32 id);
void sub_811F818(u8 partyid, u8 unk);
bool some_other_kind_of_link_test(u32 unk);
u8* get_pokesel();
u8 sub_811FEFC(u8* pokesel);
void sub_811FD88(u8 taskid, u8* pokesel);
void sub_811FBC4(u8 taskid, u8* pokesel);
void sub_8124258();
void sub_8122E5C(u8 taskid);
u8 sub_81202F8(u8* str, u8);
void sub_8123BF0(u8 taskid);

u32 pokemon_getnick(struct pokemon* pokemon, u8* dst);
void fdecoder(u8* dst, u8* src);
void bgid_mark_for_sync(u8 bgid);
void* pokemon_getattr(struct pokemon*, u8);

void callback(u8 taskid);