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

/**
  A helper function that allocates a memory pool for a particular size.
  The function will assign the new memory location to the given pointer.

  @param[in]  Size          Size of the new memory pool.
  @param[out] Buffer        The pointer to the buffer to allocate
                                   a new memory pool to.

  @retval EFI_SUCCESS       The setup is executed successfully.

**/
EFI_STATUS AllocatePoolEx(IN UINTN Size, OUT VOID **Buffer);

/**
  A function that sets up a new game structure with all of the needed field for
  a new game.

  @param[out] game          The game structure we will setup.

  @retval EFI_SUCCESS       The setup is executed successfully.

**/
EFI_STATUS setup_game(Game **game);

EFI_STATUS setup_pos(Pos **pos);

UINT32 RandomInitSeed();

void spawn_fruit(Pos *fruit_pos, UINT32 *seed);
