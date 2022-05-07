/** @file

  This is a shell application that moves a blue rectangle across the screen, just like a DVD logo.

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

// Simple Text Input Protocol
#include <Protocol/SimpleTextIn.h>

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
EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;

// Simple Text Input Protocol pointer
EFI_SIMPLE_TEXT_INPUT_PROTOCOL *sti;

// GOP video buffer
EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf;

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
  /**UINT32  Index;

  Index = 0;

  //
  // Three PCD type (FeatureFlag, UINT32 and String) are used as the sample.
  //
  if (FeaturePcdGet (PcdHelloWorldPrintEnable)) {
    for (Index = 0; Index < PcdGet32 (PcdHelloWorldPrintTimes); Index++) {
      //
      // Use UefiLib Print API to print string to UEFI console
      //
      Print ((CHAR16 *)PcdGetPtr (PcdHelloWorldPrintString));
    }
  }**/

  // Print(L"Liron Is The Best\n\n");

  // Define
  EFI_STATUS status;
  // EFI_GRAPHICS_OUTPUT_MODE_INFORMATION  *info;
  // UINTN SizeOfInfo, numModes, nativeMode;
  // UINTN numModes;

  // Locate the GOP protocol
  status = gBS->LocateProtocol(&gEfiGraphicsOutputProtocolGuid, NULL, (VOID **)&gop);
  // Print error if not found
  if (EFI_ERROR(status))
  {
    Print(L"CapsuleApp: NO GOP is found.\n");
    return EFI_UNSUPPORTED;
  }

  // Locate the STI protocol
  status = gBS->LocateProtocol(&gEfiSimpleTextInProtocolGuid, NULL, (VOID **)&sti);
  // Print error if not found
  if (EFI_ERROR(status))
  {
    Print(L"CapsuleApp: NO Simple Text Input Protocol found is found.\n");
    return EFI_UNSUPPORTED;
  }
  else
  {
    Print(L"Simple Text Input Found!\n");
  }

  EFI_INPUT_KEY *key;

  status = sti->Reset(sti, FALSE);
  if (EFI_ERROR(status))
  {
    Print(L"Can't reset\n");
  }

  while (TRUE)
  {
    while (sti->ReadKeyStroke(sti, key) != EFI_SUCCESS)
    {
      continue;
    }

    Print(L"The key pressed is: " + key->UnicodeChar + '\n');
  }

  // // Get info param
  // numModes = gop->Mode->MaxMode;

  // // Change mode
  // gop->SetMode(gop, numModes);

  // EFI_GRAPHICS_OUTPUT_BLT_PIXEL white = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Blue = 255, .Green = 255, .Red = 255};
  // EFI_GRAPHICS_OUTPUT_BLT_PIXEL blue = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Blue = 255};

  // UINT32 width = gop->Mode->Info->HorizontalResolution;
  // UINT32 height = gop->Mode->Info->VerticalResolution;

  // // vidbuf = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *) gBS->AllocatePool(width*height*sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
  // status = gBS->AllocatePool(
  //     EfiBootServicesData,
  //     width * height * sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
  //     (VOID **)&vidbuf);
  // if (EFI_ERROR(status))
  // {
  //   Print(L"Can't allocate pool.\n");
  //   return status;
  // }

  // int x = 100;
  // int y = 100;
  // int incX = width / 500;
  // int incY = height / 500;

  // Print(L"Started loop");

  // while (1)
  // {
  //   x += incX;
  //   y += incY;

  //   if (x + 50 > width || x < 1)
  //     incX *= -1;
  //   if (y + 50 > height || y < 1)
  //     incY *= -1;

  //   fill(white);

  //   draw_rect(x, y, 50, 50, blue);

  //   flip_display();
  // }

  return EFI_SUCCESS;
}
