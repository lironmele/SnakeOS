#include <Uefi.h>

#define Snake_H

#ifndef Helpers_H
#include "helpers.h"

#endif

typedef struct
{
    int score;
    enum Directions direction;
    Pos *head;
    Pos body[100];
    BOOLEAN dead;
} Snake;

/**
  A function to check if the user has pressed their keyboard.
  If the user did, the function would find out which key was
  pressed and adjust the direction accordingly.

  @param[in] stip           A pointer to the Text Input protocol, used to get the user's input.
  @param[in] inputKey       A pointer to store the newly pressed key inside.

  @param[out] direction     A pointer to a Directions enum which contains the player wants to go.

**/
VOID UpdateDirection(IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL *stip, IN EFI_INPUT_KEY *inputKey, OUT enum Directions *direction);

/**
  A function that's supposed to run every frame.
  Updates the snake's head based on the direction
  and moves the body towards the head.

  @param[in] snake           A pointer to the snake's structure, which contains the snake's information.
**/
VOID UpdateLocation(OUT Snake *snake);
