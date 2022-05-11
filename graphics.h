#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>

// Get gBS
#include <Library/UefiBootServicesTableLib.h>

// Graphics Output Protocol
#include <Protocol/GraphicsOutput.h>

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL BLACK = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Blue = 0, .Green = 0, .Red = 0};

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL GREEN = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Green = 255};

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL RED = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Red = 255};

EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel);

EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel);

EFI_STATUS flip_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);
