#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <libdragon.h>

#include "../rendering.h"

#define NUM_BOXES 26
#define NUM_BOXES_PLAYING 25
#define MIN_PRIZE 1
#define MAX_PRIZE 1000000
#define NUM_TRASH_BOXES 5
#define NUM_TRASH_NAMES 50

// Constants for box dimensions - use values from sprites or define here
#define BOX_SPRITE_WIDTH 24
#define BOX_SPRITE_HEIGHT 16

typedef struct Box
{
    int boxNumber;
    int isOpen;
    int posX;
    int posY;
    int prizeValue;
    const char *prizeDisplayName;
} Box;

Box *boxes_generate(int boxWidth, int boxHeight, int numRows, int numCols, int gap);
void boxes_recalculate_positions(Box *boxes, int boxWidth, int boxHeight, int numRows, int numCols, int gap);
void format_prize_display_name(char *buffer, int size, int prize_value);
#endif
