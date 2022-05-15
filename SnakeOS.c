/** @file
  An operating system whose purpose is to allow
  the user to play the famous game "Snake".

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SnakeOS.h"

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

VOID update_direction(enum Directions *direction, EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip, EFI_INPUT_KEY *inputKey)
{

  if (EFI_ERROR(gBS->WaitForEvent(1, &(stip->WaitForKey), 0)))
    Print(L"Crossed an error\n");

  stip->ReadKeyStroke(stip, inputKey);

  if (inputKey->UnicodeChar == 'w' || inputKey->ScanCode == 0x01)
    *direction = UP;
  else if (inputKey->UnicodeChar == 'a' || inputKey->ScanCode == 0x04)
    *direction = LEFT;
  else if (inputKey->UnicodeChar == 's' || inputKey->ScanCode == 0x02)
    *direction = DOWN;
  else if (inputKey->UnicodeChar == 'd' || inputKey->ScanCode == 0x03)
    *direction = RIGHT;

  Print(L"Updated direction: %d\n", *direction);
}

VOID EFIAPI GameLoop(
    IN EFI_EVENT Event,
    IN VOID *Context)
{
  // Protocols *protocols = ((GameContext *)Context)->protocols;
  Game *game = ((GameContext *)Context)->game;
  // int width = ((GameContext *)Context)->width;
  // int height = ((GameContext *)Context)->height;

  // paint_board(protocols->gop, vidbuf, game, width, height);

  switch (game->direction)
  {
  case UP:
    --game->head->y;
    break;
  case LEFT:
    --game->head->x;
    break;
  case DOWN:
    ++game->head->y;
    break;
  case RIGHT:
    ++game->head->x;
    break;
  }

  if (game->head->x > 24)
    game->head->x = 24;
  else if (game->head->x < 0)
    game->head->x = 0;
  else if (game->head->y > 24)
    game->head->y = 24;
  else if (game->head->y < 0)
    game->head->y = 0;

  Print(L"Game loop direction: %d\n", game->direction);
}

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

  Print(L"Started game loop\n");

  GameContext *context;
  gBS->AllocatePool(
      EfiBootServicesData,
      sizeof(GameContext),
      (VOID **)&context);

  context->game = game;
  context->protocols = protocols;
  context->width = width;
  context->height = height;

  gBS->CreateEvent(
      EVT_TIMER | EVT_NOTIFY_SIGNAL,
      TPL_NOTIFY,
      GameLoop,
      context,
      &context->PeriodicTimer);

  gBS->SetTimer(
      context->PeriodicTimer,
      TimerPeriodic,
      EFI_TIMER_PERIOD_SECONDS(1));

  EFI_INPUT_KEY *inputKey;
  gBS->AllocatePool(
      EfiBootServicesData,
      sizeof(EFI_INPUT_KEY),
      (VOID **)&inputKey);
  while (1)
  {
    update_direction(&game->direction, protocols->stip, inputKey);
  }

  return EFI_SUCCESS;
}
