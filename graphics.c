#include "graphics.h"

EFI_STATUS paint_board(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, Game *game, int width, int height)
{
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL snake_color = GREEN;
    if (game->dead == TRUE)
        snake_color = BLACK;

    int width_pixel = width / 25;
    int height_pixel = height / 25;

    // paint board
    fill(gop, vidbuf, WHITE);

    // paint head
    draw_rect(gop, vidbuf, game->head->x * width_pixel, game->head->y * height_pixel, width_pixel, height_pixel, snake_color);

    // paint body
    for (UINT32 i = 0; i < game->score; i++)
    {
        draw_rect(gop, vidbuf, game->body[i]->x * width_pixel, game->body[i]->y * height_pixel, width_pixel, height_pixel, snake_color);
    }

    // paint fruit
    draw_rect(gop, vidbuf, game->fruit->x * width_pixel, game->fruit->y * height_pixel, width_pixel, height_pixel, RED);

    // update the display
    flip_display(gop, vidbuf);

    return EFI_SUCCESS;
}

EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel)
{
    UINT32 location = y * gop->Mode->Info->HorizontalResolution + x;
    UINT32 max = gop->Mode->Info->HorizontalResolution - x;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            if (w < max)
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