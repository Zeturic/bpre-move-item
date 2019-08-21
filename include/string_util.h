#pragma once

extern u8 gStringVar1[];
extern u8 gStringVar2[];
extern u8 gStringVar3[];
extern u8 gStringVar4[];

u8 *StringAppend(u8 *dest, const u8 *src);
u8 *StringExpandPlaceholders(u8 *dest, const u8 *src);