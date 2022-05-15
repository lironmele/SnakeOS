#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>

#define Helpers_H

typedef struct
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip;
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
    UINT32 seed;
    BOOLEAN dead;
    Pos *head;
    enum Directions direction;
    Pos body[100];
    Pos *fruit;
} Game;

EFI_STATUS setup_game(Game **game);

EFI_STATUS setup_pos(Pos **pos);

UINT32 RandomInitSeed();

void spawn_fruit(Pos *fruit_pos, UINT32 *seed);
