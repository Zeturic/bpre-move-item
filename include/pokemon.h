#pragma once

#define MON_DATA_SPECIES        11
#define MON_DATA_HELD_ITEM      12
#define MON_DATA_IS_EGG         45

struct packed Pokemon
{
    u8 filler0[100];
};

extern struct Pokemon gPlayerParty[];

u32 GetMonData(struct Pokemon *mon, s32 field);
void SetMonData(struct Pokemon *mon, s32 field, const void *dataArg);
