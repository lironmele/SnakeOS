#include <Uefi.h>

#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

// Get gBS
#include <Library/UefiBootServicesTableLib.h>

// Graphics Output Protocol
#include <Protocol/GraphicsOutput.h>

#ifndef Helpers_H
#define Helpers_H
#include "helpers.h"

#endif

#ifndef Graphics_H
#define Graphics_H
#include "graphics.h"

#endif

typedef struct
{
  UINTN Signature;
  EFI_EVENT PeriodicTimer;
  Game *game;
  Protocols *protocols;
  UINT32 width;
  UINT32 height;
} GameContext;

/**
  A function to check if the user has pressed their keyboard.
  If the user did, the function would find out which key was
  pressed and adjust the direction accordingly.

  @param[in] stip           A pointer to the Text Input protocol, used to get the user's input.
  @param[in] inputKey       A pointer to store the newly pressed key inside.

  @param[out] direction     A pointer to a Directions enum which contains the player wants to go.

**/
VOID UpdateDirection(IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip, IN EFI_INPUT_KEY *inputKey, OUT enum Directions *direction);

/**
  A function that's supposed to run every frame.
  Updates the snake's head based on the direction
  and moves the body towards the head.

  @param[in] game           A pointer to the game's structure, which contains the snake's information.
**/
VOID UpdateLocation(OUT Game *game);

/**
  A function that is supposed to run every frame.
  Houses the game's logic.

  @param[out]  Event        The pointer to the newly created event if the call succeeds; undefined
                                otherwise.
  @param[in]   Context      The pointer to the notification function's context; corresponds to parameter
                                Context in the notification function.
**/
VOID EFIAPI GameLoop(
    IN EFI_EVENT Event,
    IN VOID *Context);

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable);