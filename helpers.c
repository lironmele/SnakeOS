#include "helpers.h"

EFI_STATUS AllocatePoolEx(IN UINTN Size, OUT VOID **Buffer)
{
    EFI_STATUS status;

    status = gBS->AllocatePool(
        EfiBootServicesData,
        Size,
        (VOID **)Buffer);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool. Size of attempted allocation: %d.\n", Size);
        return status;
    }

    return EFI_SUCCESS;
}

EFI_STATUS SetupProtocols(Protocols **protocols)
{
    Print(L"Loading protocols\n");

    EFI_STATUS status;

    status = AllocatePoolEx(sizeof(Protocols), (VOID **)protocols);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate pool for protocols struct.\n");
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

    Print(L"Loaded all protocols successfully.\n");
    return EFI_SUCCESS;
}

EFI_STATUS SetupGame(Game **game)
{
    Print(L"Setting up game\n");

    EFI_STATUS status;

    status = AllocatePoolEx(sizeof(Game), (VOID **)game);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate pool for game struct.\n");
        return status;
    }

    // Create variables for game pointer and snake pointer for better readability
    Game *game_ptr = *game;
    Snake *snake_ptr = game_ptr->snake;

    status = AllocatePoolEx(sizeof(Snake), (VOID **)&game_ptr->snake);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate pool for snake struct");
        return status;
    }

    status = AllocatePoolEx(sizeof(Pos) * 100, (VOID **)&snake_ptr->body);
    if (EFI_ERROR(status))
    {
        Print(L"Failed to allocate pool for snake's body struct.\n");
        return status;
    }

    // Setup the player's starting position
    status = AllocatePoolEx(sizeof(Pos), (VOID **)&snake_ptr->head);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for player's Pos struct.\n");
        return status;
    }
    else
    {
        snake_ptr->head->x = 15;
        snake_ptr->head->y = 15;
    }

    // Generate a random seed for future random generation
    game_ptr->seed = RandomInitSeed();

    // Initialize the player to be alive
    snake_ptr->dead = FALSE;

    // The player's starting direction is right.
    snake_ptr->direction = RIGHT;

    // Start the score at 0
    snake_ptr->score = 0;

    // Setup the fruit in a random starting position

    status = AllocatePoolEx(sizeof(Pos), (VOID **)&game_ptr->fruit);
    if (EFI_ERROR(status))
    {
        Print(L"Can't allocate pool for fruit's Pos struct.");
        return status;
    }
    else
    {
        SpawnFruit(game_ptr->fruit, &game_ptr->seed);
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

VOID SpawnFruit(Pos *fruit_pos, UINT32 *seed)
{
    *seed -= 7;
    fruit_pos->x = (*seed * 3) % 25;
    fruit_pos->y = *seed % 25;
}