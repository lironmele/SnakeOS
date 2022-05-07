/** @file
  An operating system whose purpose is to allow
  the user to play the famous game "Snake".

  Copyright (c) 2006 - 2018, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

// Get gBS
#include <Library/UefiBootServicesTableLib.h>

// Graphics Output Protocol
#include <Protocol/GraphicsOutput.h>

// Simple Pointer Protocol
#include <Protocol/SimplePointer.h>

//
// String token ID of help message text.
// Shell supports to find help message in the resource section of an application image if
// .MAN file is not found. This global variable is added to make build tool recognizes
// that the help string is consumed by user and then build tool will add the string into
// the resource section. Thus the application can use '-?' option to show help message in
// Shell.
//
// GLOBAL_REMOVE_IF_UNREFERENCED EFI_STRING_ID mStringHelpTokenId = STRING_TOKEN(STR_HELLO_WORLD_HELP_INFORMATION);

// GOP pointer
static EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

// Simple Pointer Protocol pointer
static EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip;

EFI_STATUS draw_rect(UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel)
{
  UINT32 location = y * gop->Mode->Info->HorizontalResolution + x;

  for (int h = 0; h < height; h++)
  {
    for (int w = 0; w < width; w++)
    {
      vidbuf[location] = pixel;
      location++;
    }
    location -= width;
    location += gop->Mode->Info->HorizontalResolution;
  }

  return EFI_SUCCESS;
}

EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel)
{
  UINT32 width = gop->Mode->Info->HorizontalResolution;
  UINT32 height = gop->Mode->Info->VerticalResolution;
  for (UINT32 i = 0; i < width * height; i++)
  {
    vidbuf[i] = pixel;
  }

  return EFI_SUCCESS;
}

EFI_STATUS flip_display()
{
  return gop->Blt(gop, vidbuf, EfiBltBufferToVideo, 0, 0, 0, 0, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, 0);
}

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
    IN EFI_SYSTEM_TABLE *SystemTable)
{
  // Define
  EFI_STATUS status;
  UINTN numModes;

  // Locate the GOP protocol
  status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gop);
  // Print error if not found
  if (EFI_ERROR(status))
  {
    Print(L"CapsuleApp: NO GOP is found.\n");
    return EFI_UNSUPPORTED;
  }

  // Get info param
  numModes = gop->Mode->MaxMode;

  // Change mode
  gop->SetMode(gop, numModes);

  EFI_GRAPHICS_OUTPUT_BLT_PIXEL black = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Red = 0, .Green = 0, .Blue = 0};
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL green = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Green = 255};
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL red = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Red = 255};

  UINT32 width = gop->Mode->Info->HorizontalResolution;
  UINT32 height = gop->Mode->Info->VerticalResolution;

  status = gBS->AllocatePool(
      EfiBootServicesData,
      width * height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
      (VOID **)&vidbuf);
  if (EFI_ERROR(status))
  {
    Print(L"Can't allocate pool.\n");
    return status;
  }

  status = gBS->LocateProtocol(&gEfiSimpleTextInProtocolGuid, NULL, (VOID **)&stip);
  if (EFI_ERROR(status))
  {
    Print(L"Can't get STIP pointer.\n");
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
    fill(black);

    draw_rect(x, y, 25, 25, green);
    draw_rect(x + 27, y, 25, 25, green);
    draw_rect(x + 54, y, 25, 25, green);
    draw_rect(x + 81, y, 25, 25, green);
    draw_rect(x + 108, y, 25, 25, green);

    draw_rect(300, 250, 25, 25, red);

    flip_display();

    if (EFI_ERROR(gBS->WaitForEvent(1, &stip->WaitForKey, 0)))
      Print(L"Crossed an error");

    stip->ReadKeyStroke(stip, inputKey);

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
