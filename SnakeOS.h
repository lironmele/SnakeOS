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