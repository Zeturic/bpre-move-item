#pragma once
#include "types.h"

u8 *StringAppend(u8 *dest, const u8 *src);
u8 *StringExpandPlaceholders(u8 *dest, const u8 *src);

u32 GetMonData(struct Pokemon *mon, s32 field);
void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg);

void PlaySE(u16 songNum);
void CopyItemName(u16 itemId, u8 *dst);
bool8 some_other_kind_of_link_test(void);
void schedule_bg_copy_tilemap_to_vram(u8 bgNum);

void sub_811F818(u8 slot, u8 b);
void sub_811FD88(u8 taskId, s8 *ptr);
u16 PartyMenuButtonHandler(s8 *ptr);
u8* GetMonNickname(struct Pokemon *mon, u8 *dest);
u8 sub_81202F8(const u8* str, u8 b);
void display_pokemon_menu_message(u32 stringID);
void sub_8121CE4(u8 *ptr);
void sub_81224B4(struct Pokemon *mon, struct Struct203B0B4 *ptr);
void sub_8123BF0(u8 taskId);