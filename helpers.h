#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>
// #include <Protocol/Rng.h>
// #include "BaseRngLib/Rand/RdRand.h"

#define Helpers_H

typedef struct
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip;
    // EFI_RNG_PROTOCOL *rng;
} Protocols;

EFI_STATUS get_protocols(Protocols **protocols);

typedef struct
{
    int x;
    int y;
} Pos;

enum Directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

typedef struct
{
    int score;
    BOOLEAN dead;
    Pos *head;
    enum Directions direction;
    Pos *body[100];
    Pos *fruit;
} Game;

// EFI_STATUS setup_game(Game **game, EFI_RNG_PROTOCOL *rng);

// void spawn_fruit(EFI_RNG_PROTOCOL *rng, Pos *fruit_pos);

EFI_STATUS setup_game(Game **game);

EFI_STATUS setup_pos(Pos **pos);

int RandomInitSeed(VOID);

void spawn_fruit(Pos *fruit_pos);
