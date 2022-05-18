/** @file
  An operating system whose purpose is to allow
  the user to play the famous game "Snake".

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SnakeOS.h"

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

VOID EFIAPI GameLoop(
    IN EFI_EVENT Event,
    IN VOID *Context)
{
  // Setup variables for better readability
  Protocols *protocols = ((GameContext *)Context)->protocols;
  Game *game = ((GameContext *)Context)->game;
  int width = ((GameContext *)Context)->width;
  int height = ((GameContext *)Context)->height;
  Snake *snake = game->snake;

  // Update the frame
  paint_board(protocols->gop, game, width, height, vidbuf);

  // If the player is dead, don't update the game's state
  if (snake->dead)
    return;

  // Update the snake's location
  UpdateLocation(game->snake);

  // Check if the snake had eaten the fruit
  if (
      snake->head->x == game->fruit->x &&
      snake->head->y == game->fruit->y)
  {
    ++snake->score;
    SpawnFruit(game->fruit, &game->seed);
  }

  // Check for the snake's collision with its body
  for (int i = 0; i < snake->score; i++)
  {
    if (snake->head->x == snake->body[i].x &&
        snake->head->y == snake->body[i].y)
    {
      snake->dead = TRUE;
      return;
    }
  }

  // Check for the snake's collision with its head.
  int x = snake->head->x;
  int y = snake->head->y;
  if (x < 0)
    snake->head->x = 0;
  else if (x > 24)
    snake->head->x = 24;
  else if (y < 0)
    snake->head->y = 0;
  else if (y > 24)
    snake->head->y = 24;
  else
    return;
  snake->dead = TRUE;
}

EFI_STATUS
EFIAPI
UefiMain(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  EFI_STATUS status;

  Protocols *protocols;
  status = SetupProtocols(&protocols);
  if (EFI_ERROR(status))
  {
    Print(L"Failed to load protocols. GOODBYE!\n");
    return status;
  }

  Game *game;
  status = SetupGame(&game);
  if (EFI_ERROR(status))
  {
    Print(L"Faild to setup the game. GOODBYE!\n");
    return status;
  }

  // Change display resolution to highest
  protocols->gop->SetMode(protocols->gop, protocols->gop->Mode->MaxMode);

  UINT32 width = protocols->gop->Mode->Info->HorizontalResolution;
  UINT32 height = protocols->gop->Mode->Info->VerticalResolution;

  status = AllocatePoolEx(width * height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL), (VOID **)&vidbuf);
  if (EFI_ERROR(status))
  {
    Print(L"Can't allocate pool for video buffer.\n");
    return status;
  }

  Print(L"Started game loop.\n");

  GameContext *context;
  status = AllocatePoolEx(sizeof(GameContext), (VOID **)&context);
  if (EFI_ERROR(status))
  {
    Print(L"Failed to allocate pool for GameContext structure.\n");
  }

  // Assign all relevant field to context
  context->game = game;
  context->protocols = protocols;
  context->width = width;
  context->height = height;

  // Create a new event for the game loop function
  gBS->CreateEvent(
      EVT_TIMER | EVT_NOTIFY_SIGNAL,
      TPL_NOTIFY,
      GameLoop,
      context,
      &context->PeriodicTimer);

  // Set a timer for 0.125 milliseconds for the gameloop
  gBS->SetTimer(
      context->PeriodicTimer,
      TimerPeriodic,
      EFI_TIMER_PERIOD_MILLISECONDS(125));

  EFI_INPUT_KEY inputKey;

  while (1)
  {
    // Check for user input constantly
    UpdateDirection(protocols->stip, &inputKey, &game->snake->direction);
  }

  return EFI_SUCCESS;
}
