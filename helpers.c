#include "helpers.h"

EFI_STATUS get_protocols(Protocols **protocols)
{
    Print(L"Loading protocols\n");

    EFI_STATUS status;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        sizeof(Protocols),
        (VOID **)protocols);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for protocols struct.\n");
        return status;
    }

    status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)(&((*protocols)->gop)));

    if (EFI_ERROR(status))
    {
        Print(L"Can't get Graphics Output Protocol pointer.\n");
        return status;
    }

    status = gBS->LocateProtocol(&gEfiSimpleTextInProtocolGuid, NULL, (VOID **)(&((*protocols)->stip)));

    if (EFI_ERROR(status))
    {
        Print(L"Can't get Simple Text Input Protocol pointer.\n");
        return status;
    }

    Print(L"Got all protocols\n");
    return EFI_SUCCESS;
}

EFI_STATUS setup_game(Game **game)
{
    Print(L"Setting up game\n");

    EFI_STATUS status;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        sizeof(Game),
        (VOID **)game);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for game struct.\n");
        return status;
    }

    Print(L"Allocated pool\n");

    Game *game_ptr = *game;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        sizeof(Pos) * 100,
        (VOID **)&game_ptr->body);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for game struct.\n");
        return status;
    }

    setup_pos(&game_ptr->head);

    game_ptr->seed = RandomInitSeed();

    game_ptr->dead = FALSE;

    game_ptr->direction = RIGHT;

    setup_pos(&game_ptr->fruit);

    game_ptr->score = 0;

    game_ptr->head->x = 15;
    game_ptr->head->y = 15;

    spawn_fruit(game_ptr->fruit, &game_ptr->seed);

    return EFI_SUCCESS;
}

EFI_STATUS setup_pos(Pos **pos)
{
    EFI_STATUS status;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        sizeof(Pos),
        (VOID **)pos);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for Pos struct.\n");
        return status;
    }

    return EFI_SUCCESS;
}

UINT32 RandomInitSeed(VOID)
{
    EFI_TIME Time;
    UINT32 Seed;
    gRT->GetTime(&Time, NULL);
    Seed = (~Time.Hour << 24 | Time.Day << 16 | Time.Minute << 8 | Time.Second);
    Seed ^= Time.Nanosecond;
    Seed ^= Time.Year << 7;
    return Seed;
}

VOID spawn_fruit(Pos *fruit_pos, UINT32 *seed)
{
    *seed -= 7;
    fruit_pos->x = (*seed * 3) % 25;
    fruit_pos->y = *seed % 25;
}