#include "snake.h"

VOID UpdateDirection(IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip, IN EFI_INPUT_KEY *inputKey, OUT enum Directions *direction)
{
    if (EFI_ERROR(gBS->CheckEvent(stip->WaitForKey)))
        return;

    stip->ReadKeyStroke(stip, inputKey);

    if (inputKey->UnicodeChar == 'w' || inputKey->ScanCode == 0x01)
        *direction = UP;
    else if (inputKey->UnicodeChar == 'a' || inputKey->ScanCode == 0x04)
        *direction = LEFT;
    else if (inputKey->UnicodeChar == 's' || inputKey->ScanCode == 0x02)
        *direction = DOWN;
    else if (inputKey->UnicodeChar == 'd' || inputKey->ScanCode == 0x03)
        *direction = RIGHT;
}

VOID UpdateLocation(OUT Snake *snake)
{
    for (int i = snake->score - 1; i > 0; i--)
    {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0] = *snake->head;

    switch (snake->direction)
    {
    case UP:
        --snake->head->y;
        break;
    case LEFT:
        --snake->head->x;
        break;
    case DOWN:
        ++snake->head->y;
        break;
    case RIGHT:
        ++snake->head->x;
        break;
    }
}
