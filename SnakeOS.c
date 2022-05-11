/** @file
  An operating system whose purpose is to allow
  the user to play the famous game "Snake".

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include "SnakeOS.h"
#include "graphics.h"
#include "setup.h"

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

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
    Print(L"Failed to load protocols. GOODBYE!");
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
    Print(L"Can't allocate pool.\n");
    return status;
  }

  int x = 100;
  int y = 100;
  int incX = width / 100;
  int incY = height / 100;

  EFI_INPUT_KEY *inputKey;

  Print(L"Started loop");

  while (1)
  {
    fill(protocols->gop, vidbuf, BLACK);

    draw_rect(protocols->gop, vidbuf, x, y, 25, 25, GREEN);

    flip_display(protocols->gop, vidbuf);

    if (EFI_ERROR(gBS->WaitForEvent(1, &(protocols->stip->WaitForKey), 0)))
      Print(L"Crossed an error");

    protocols->stip->ReadKeyStroke(protocols->stip, inputKey);

    if (inputKey->UnicodeChar == 'w' || inputKey->ScanCode == 0x01)
      y -= incY;
    else if (inputKey->UnicodeChar == 'a' || inputKey->ScanCode == 0x04)
      x -= incX;
    else if (inputKey->UnicodeChar == 's' || inputKey->ScanCode == 0x02)
      y += incY;
    else if (inputKey->UnicodeChar == 'd' || inputKey->ScanCode == 0x03)
      x += incX;

    if (x + 50 > width)
      x = width - 50;
    else if (x < 0)
      x = 0;
    else if (y + 50 > height)
      y = height - 50;
    else if (y < 0)
      y = 0;
  }

  return EFI_SUCCESS;
}
