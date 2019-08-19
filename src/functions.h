#pragma once
#include "types.h"

void PlaySE(u16 songNum);
void sub_8121CE4(u8* unk);
void display_pokemon_menu_message(u32 id);
void sub_811F818(u8 partyid, u8 unk);
bool8 some_other_kind_of_link_test(u32 unk);
s8* get_pokesel();
u16 PartyMenuButtonHandler(s8 *ptr);
void sub_811FD88(u8 taskid, s8* pokesel);
void sub_811FBC4(u8 taskid, s8* pokesel);
void sub_8124258();
void sub_8122E5C(u8 taskid);
u8 sub_81202F8(u8* str, u8);
void sub_8123BF0(u8 taskid);
void sub_8123270(u8 taskid);
void sub_81231E8(u8 taskid);
void DisplayPartyPokemonData(u8 taskid);

u32 GetMonNick(struct Pokemon* pokemon, u8* dst);
u8 *StringExpandPlaceholders(u8 *dest, const u8 *src);
void schedule_bg_copy_tilemap_to_vram(u8 bgid);
void RenderPartyMenuBox(u8 slot);
void InitPartyMenu(u8, u8, u8, u8, u8, void (*task)(), void (*callback)());
void sub_81224B4(struct Pokemon*, struct Struct203B0B4*);
void AddTextPrinterParameterized3(u8 windowId, u8 fontId, u8 left, u8 top, const u8 *color, s8 speed, const u8 *str);
void HandleMenuInput(u8 taskId);
void AppendToList(u8* list, u8* pos, u8 newEntry);
void CreateItemActionList(struct Pokemon *mons);
u16 AddWindow(const struct WindowTemplate *template);
u8 sub_8121DF8(void);
void sub_8121CE4(u8 *ptr);
void DrawStdFrameWithCustomTileAndPalette(u8 windowId, bool8 copyToVram, u16 tileStart, u8 palette);
u16 AddTextPrinterParameterized(u8 windowId, u8 fontId, const u8 *str, u8 x, u8 y, u8 speed, void (*callback)(struct TextPrinterTemplate *, u16));

u32 GetMonData(struct Pokemon*, u8);
void* SetMonData(struct Pokemon*, u8, void*);

void sub_8121CE4(u8 *ptr);
void sub_812299C(struct Pokemon *mons, u8 slotId, u8 b);
u8 sub_8121E5C(u8 a);

void callback(u8 taskid);

void bkpt(u32 x);