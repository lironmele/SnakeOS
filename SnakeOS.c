/** @file
  An operating system whose purpose is to allow
  the user to play the famous game "Snake".

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SnakeOS.h"

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

// Direction
enum Directions direction;

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Define
  EFI_STATUS status;

  Protocols *protocols;
  status = get_protocols(&protocols);
  if (EFI_ERROR(status))
  {
    Print(L"Failed to load protocols. GOODBYE!\n");
    return status;
  }

  Game *game;
  status = setup_game(&game);
  if (EFI_ERROR(status))
  {
    Print(L"Faild to setup the game. GOODBYE!\n");
    return status;
  }

  // Change display resolution to highest
  protocols->gop->SetMode(protocols->gop, protocols->gop->Mode->MaxMode);

  UINT32 width = protocols->gop->Mode->Info->HorizontalResolution;
  UINT32 height = protocols->gop->Mode->Info->VerticalResolution;

  status = gBS->AllocatePool(
      EfiBootServicesData,
      width * height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
      (VOID **)&vidbuf);
  if (EFI_ERROR(status))
  {
    Print(L"Can't allocate pool for video buffer.\n");
    return status;
  }

  EFI_INPUT_KEY *inputKey;

  Print(L"Started loop\n");

  while (1)
  {
    paint_board(protocols->gop, vidbuf, game, width, width);

    if (EFI_ERROR(gBS->WaitForEvent(1, &(protocols->stip->WaitForKey), 0)))
      Print(L"Crossed an error\n");

    protocols->stip->ReadKeyStroke(protocols->stip, inputKey);

    if (inputKey->UnicodeChar == 'w' || inputKey->ScanCode == 0x01)
      --game->head->y;
    else if (inputKey->UnicodeChar == 'a' || inputKey->ScanCode == 0x04)
      --game->head->x;
    else if (inputKey->UnicodeChar == 's' || inputKey->ScanCode == 0x02)
      ++game->head->y;
    else if (inputKey->UnicodeChar == 'd' || inputKey->ScanCode == 0x03)
      ++game->head->x;

    if (game->head->x > 24)
      game->head->x = 24;
    else if (game->head->x < 0)
      game->head->x = 0;
    else if (game->head->y > 24)
      game->head->y = 24;
    else if (game->head->y < 0)
      game->head->y = 0;
  }

  return EFI_SUCCESS;
}
