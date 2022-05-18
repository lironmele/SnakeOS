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

/**
  Paint the current state of the board onto a video buffer.

  @param[in] gop            A pointer to the GOP instance used to determine the screen's size.
  @param[in] game           A pointer to the game's structure, used to determine the curret state.
  @param[in] width          The width of the board.
  @param[in] height         The height of the board.

  @param[out] vidbuf        A buffer which the new board will be drawn onto.
**/
EFI_STATUS paint_board(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, Game *game, int width, int height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);

/**
  Draw a filled in rectangle of a single color onto a video buffer.

  @param[in] gop            A pointer to the GOP instance used to determine the screen's size.
  @param[in] x              The x value of the left upper corner of the rectangle.
  @param[in] y              The y value of the left upper corner of the rectangle.
  @param[in] width          The width of the rectangle.
  @param[in] height         The height of the rectangle.
  @param[in] pixel          The pixel which the rectangle will be created from.

  @param[out] vidbuf        A buffer which the rectangle will be drawn onto.
**/
EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);

/**
  Fill the entire screen with one color.

  @param[in] gop            A pointer to the GOP instance used to determine the screen's size.
  @param[in] pixel          The pixel we want to fill the screen with.

  @param[out] vidbuf        A buffer which will be filled with the specified pixel.
**/
EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);

/**
  Flip the display. This Writes the video buffer to the screen and draws a new frame.

  @param[in] gop            A pointer to the GOP instance used to Blit new pixels to the screen.
  @param[in] vidbuf         A buffer full of pixels which will be copied onto the screen.
**/
EFI_STATUS flip_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf);
