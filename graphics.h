#include <Uefi.h>
#include <Library/PcdLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/GraphicsOutput.h>

#define Graphics_H

#ifndef Helpers_H
#include "helpers.h"

#endif

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL BLACK = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Blue = 0, .Green = 0, .Red = 0};

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL WHITE = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Blue = 255, .Green = 255, .Red = 255};

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL GREEN = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Green = 255};

static const EFI_GRAPHICS_OUTPUT_BLT_PIXEL RED = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL){.Red = 255};

EFI_STATUS paint_board(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, Game *game, int width, int height);

EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel);

EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel);

EFI_STATUS flip_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);
