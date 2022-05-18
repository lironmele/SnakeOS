#include "graphics.h"

EFI_STATUS paint_board(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, Game *game, int width, int height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf)
{
    Snake *snake = game->snake;

    // Change the snake's color based on its state
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL snake_color = GREEN;
    if (snake->dead == TRUE)
        snake_color = BLACK;

    // Calculate each tile's pixel count
    int width_pixel = width / 25;
    int height_pixel = height / 25;

    // paint board
    fill(gop, WHITE, vidbuf);

    // paint head
    draw_rect(gop, snake->head->x * width_pixel, snake->head->y * height_pixel, width_pixel, height_pixel, snake_color, vidbuf);

    // paint body
    for (UINT32 i = 0; i < snake->score; i++)
    {
        draw_rect(gop, snake->body[i].x * width_pixel, snake->body[i].y * height_pixel, width_pixel, height_pixel, snake_color, vidbuf);
    }

    // paint fruit
    draw_rect(gop, game->fruit->x * width_pixel, game->fruit->y * height_pixel, width_pixel, height_pixel, RED, vidbuf);

    // update the display
    flip_display(gop, vidbuf);

    return EFI_SUCCESS;
}

EFI_STATUS draw_rect(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 width, UINT32 height, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf)
{
    // Since the video buffer is a linear array we need to convert the two dimensional location to one dimension
    UINT32 location = y * gop->Mode->Info->HorizontalResolution + x;

    // Find out whats the maximum width we can have without going outside the screen
    UINT32 max = gop->Mode->Info->HorizontalResolution - x;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // If pixels are outside of the screen, don't draw them
            if (w < max)
                vidbuf[location] = pixel;
            location++;
        }
        location -= width;
        location += gop->Mode->Info->HorizontalResolution;
    }

    return EFI_SUCCESS;
}

EFI_STATUS fill(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, EFI_GRAPHICS_OUTPUT_BLT_PIXEL pixel, EFI_GRAPHICS_OUTPUT_BLT_PIXEL *vidbuf)
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