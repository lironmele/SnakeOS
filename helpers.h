#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/SimpleTextIn.h>

#define Helpers_H

#ifndef Snake_H
#include "snake.h"

#endif

typedef struct
{
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip;
} Protocols;

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
    UINT32 seed;
    Pos *fruit;
    Snake *snake;
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
  A function that sets up a new protocols structure and
  initialises all of the protocols in it.

  @param[out] protocols     A pointer to the buffer in which the
                            protocols will be created.

  @retval EFI_SUCCESS       The setup is executed successfully.
  @retval other             Some error occurs when executing this entry point.
**/
EFI_STATUS SetupProtocols(Protocols **protocols);

/**
  A function that sets up a new game structure with all of the needed field for
  a new game.

  @param[out] game          The game structure we will setup.

  @retval EFI_SUCCESS       The setup is executed successfully.
  @retval other             Some error occurs when executing this entry point.
**/
EFI_STATUS SetupGame(Game **game);

/**
  Generate a new random seed. Used for seeding random number generators.

  @retval UINT32            Returns the new random seed.
**/
UINT32 RandomInitSeed(VOID);

/**
  A function that generates a new random position for a fruit based on a random seed.

  @param[out] fruit_pos     A pointer to the fruit's position. The function will updated
                            those positions to new random ones.
  @param[out] seed          The seed used to generate a new position for the fruit.
                            The function updates the seed in the process.
**/
VOID SpawnFruit(Pos *fruit_pos, UINT32 *seed);
