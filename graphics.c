#include "graphics.h"

EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel)
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

EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel)
{
    UINT32 width = gop->Mode->Info->HorizontalResolution;
    UINT32 height = gop->Mode->Info->VerticalResolution;
    for (UINT32 i = 0; i < width * height; i++)
    {
        vidbuf[i] = pixel;
    }

    return EFI_SUCCESS;
}

EFI_STATUS flip_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf)
{
    return gop->Blt(gop, vidbuf, EfiBltBufferToVideo, 0, 0, 0, 0, gop->Mode->Info->HorizontalResolution, gop->Mode->Info->VerticalResolution, 0);
}