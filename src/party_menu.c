#include "global.h"

void CursorCb_MoveItem(u8 taskid) {
    struct Task* task = &gTasks[taskid];

    PlaySE(SE_SELECT);

    sub_8121CE4(&gUnknown_0203B09C->windowId[1]);
    sub_8121CE4(&gUnknown_0203B09C->windowId[0]);

    struct Pokemon* pokemon = &gPlayerParty[gUnknown_0203B0A0.slotId];

    if (GetMonData(pokemon, MON_DATA_HELD_ITEM) != ITEM_NONE) {
        gUnknown_0203B0A0.field_B = 8;

        display_pokemon_menu_message(MSG_MOVE);
        sub_811F818(gUnknown_0203B0A0.slotId, 1);
        gUnknown_0203B0A0.pokesel2 = gUnknown_0203B0A0.slotId;
        task->func = callback;

    } else {
        GetMonNick(pokemon, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_PkmnNotHolding);
        sub_81202F8(gStringVar4, 1);

        schedule_bg_copy_tilemap_to_vram(2);
        task->func = sub_8123BF0;
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
            if (button_unknown == 8 && gUnknown_0203B09C->field_8_0) {
                PlaySE(SE_SELECT);
                sub_8124258();
            }
        } else if (button_unknown == A_BUTTON) {
            PlaySE(SE_SELECT);

            u16 item1 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM);
            u16 item2 = GetMonData(&gPlayerParty[gUnknown_0203B0A0.pokesel2], MON_DATA_HELD_ITEM);

            SetMonData(&gPlayerParty[gUnknown_0203B0A0.slotId], MON_DATA_HELD_ITEM, &item2);
            SetMonData(&gPlayerParty[gUnknown_0203B0A0.pokesel2], MON_DATA_HELD_ITEM, &item1);

            sub_81224B4(
                &gPlayerParty[gUnknown_0203B0A0.slotId],
                &gUnknown_0203B0B4[gUnknown_0203B0A0.slotId]
            );

            sub_81224B4(
                &gPlayerParty[gUnknown_0203B0A0.pokesel2],
                &gUnknown_0203B0B4[gUnknown_0203B0A0.pokesel2]
            );

            sub_811FD88(taskid, pokesel);
        }
    }
}

void CursorCb_Item(u8 taskId)
{
    PlaySE(SE_SELECT);
    sub_8121CE4(&gUnknown_0203B09C->windowId[0]);
    sub_8121CE4(&gUnknown_0203B09C->windowId[1]);
    
    // sub_812299C(gPlayerParty, gUnknown_0203B0A0.slotId, 8);
    // sub_8121E5C(1);
    // display_pokemon_menu_message(25);
    
    CreateItemActionList(gPlayerParty);
    sub_8121E5C(1);
    display_pokemon_menu_message(25);

    gTasks[taskId].data[0] = 0xFF;
    gTasks[taskId].func = HandleMenuInput;
}

void CreateItemActionList(struct Pokemon *mons)
{
    gUnknown_0203B09C->listSize = 0;

    AppendToList(gUnknown_0203B09C->actions, &gUnknown_0203B09C->listSize, MENU_GIVE);
    AppendToList(gUnknown_0203B09C->actions, &gUnknown_0203B09C->listSize, MENU_TAKE_ITEM);

    if (GetMonData(&mons[1], MON_DATA_SPECIES) != SPECIES_NONE)
        AppendToList(gUnknown_0203B09C->actions, &gUnknown_0203B09C->listSize, MENU_MOVE);

    AppendToList(gUnknown_0203B09C->actions, &gUnknown_0203B09C->listSize, MENU_CANCEL2);
}

// TODO: Remove reimplementation
void display_pokemon_menu_message(u32 stringID)
{
    u8 *windowPtr = &gUnknown_0203B09C->windowId[1];

    if (*windowPtr != 0xFF)
        sub_8121CE4(windowPtr);

    if (stringID != 0x7F)
    {
        switch (stringID)
        {
        case 22:
            *windowPtr = AddWindow(&gUnknown_0845A128);
            break;
        case 23:
        case 24:
            *windowPtr = AddWindow(&gUnknown_0845A140);
            break;
        case 25:
            *windowPtr = AddWindow(&gUnknown_0845A130);
            break;
        case 26:
            *windowPtr = AddWindow(&gUnknown_0845A138);
            break;
        default:
            *windowPtr = AddWindow(&gUnknown_0845A120);
            break;
        }
        if (stringID == 0)
        {
            if (gUnknown_0203B09C->field_8_0)
                stringID = 2;
            else if (sub_8121DF8() == FALSE)
                stringID = 1;
        }
        DrawStdFrameWithCustomTileAndPalette(*windowPtr, FALSE, 0x58, 0xF);
        StringExpandPlaceholders(gStringVar4, sActionStringTable[stringID]);
        AddTextPrinterParameterized(*windowPtr, 2, gStringVar4, 0, 2, 0, 0);
        schedule_bg_copy_tilemap_to_vram(2);
    }
}

u8 sub_8121E5C(u8 a)
{
    struct WindowTemplate window;
    u8 cursorDimension;
    u8 fontAttribute;
    u8 i;

    switch (a)
    {
    case 0:
        SetWindowTemplateFields(&window, 2, 19, 19 - (gUnknown_0203B09C->listSize * 2), 10, gUnknown_0203B09C->listSize * 2, 14, 0x2BF);
        break;
    case 1:
        window = gUnknown_0845A148;
        break;
    case 2:
        window = gUnknown_0845A150;
        break;
    default:
        window = gUnknown_0845A158;
        break;
    }

    gUnknown_0203B09C->windowId[0] = AddWindow(&window);
    DrawStdFrameWithCustomTileAndPalette(gUnknown_0203B09C->windowId[0], FALSE, 0x4F, 13);
    if (a == 3)
        return gUnknown_0203B09C->windowId[0];
    cursorDimension = GetMenuCursorDimensionByFont(2, 0);
    fontAttribute = GetFontAttribute(2, 2);

    for (i = 0; i < gUnknown_0203B09C->listSize; i++)
    {
        u8 unk = (gUnknown_0203B09C->actions[i] > 18) ? 4 : 3;
        AddTextPrinterParameterized4(gUnknown_0203B09C->windowId[0], 1, cursorDimension, (i * 16) + 1, fontAttribute, 0, sFontColorTable[unk], 0, sCursorOptions[gUnknown_0203B09C->actions[i]].text);
    }

    choice_setup(gUnknown_0203B09C->windowId[0], 2, 0, 2, 16, gUnknown_0203B09C->listSize, 0, 1);
    schedule_bg_copy_tilemap_to_vram(2);

    return gUnknown_0203B09C->windowId[0];
}