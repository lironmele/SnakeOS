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

VOID update_direction(enum Directions *direction, EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip, EFI_INPUT_KEY *inputKey);

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