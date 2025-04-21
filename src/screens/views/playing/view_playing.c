#include "view_playing.h"
#include "../../../state/box.h" // Include box.h for BOX_SPRITE_* defines
#include <rdpq.h> // Include RDPQ
#include "../../../fonts.h" // Include the new font definitions

// Define font ID (assuming it's 1 as set in game.c)
// #define FONT_PACIFICO 1 // Replaced by constants in fonts.h

// Remove helper function, moved to screen_playing.c
/*
static bool is_playing_box_cursor_valid(int cursorIndex) {
    return cursorIndex >= 0 && cursorIndex < NUM_BOXES_PLAYING;
}
*/

// Use ScreenPlayingState* from the struct definition now
void view_playing_draw(ScreenPlayingState *pScreenState, sprite_t *box_unsealed, sprite_t *box_sealed, sprite_t *cursor)
{
    // Add null checks at beginning
    if (!pScreenState || !box_sealed || !box_unsealed || !cursor) {
        // Static string
        rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 35, 30, "^00ERROR: Invalid sprites or state.");
        return;
    }
    
    // Static string
    rdpq_text_printf(NULL, FONT_SIZE_H1, 115, 15, "^00OPEN A BOX");

    /* Draw the boxes */
    sprite_t *box_sprite;
    // Draw NUM_BOXES_PLAYING boxes
    for (int i = 0; i < NUM_BOXES_PLAYING; i++)
    {
        Box *box = &(pScreenState->boxes[i]);

        if (box->isOpen)
        {
            box_sprite = box_unsealed;
        }
        else
        {
            box_sprite = box_sealed;
        }

        // Use RDPQ for sprite drawing
        rdpq_sprite_blit(box_sprite, box->posX, box->posY, NULL);

        if (box->isOpen)
        {
            // Only draw prize if it was the last opened box
             if (box->boxNumber == pScreenState->lastOpenedBoxNumber)
             {
                // Box is open, draw the prize using RDPQ
                 if(box->prizeDisplayName) {
                    rdpq_text_printf(NULL, FONT_SIZE_BODY, box->posX - BOX_SPRITE_WIDTH / 2, box->posY + BOX_SPRITE_HEIGHT + 5, "^00%s", box->prizeDisplayName);
                 }
             }
            continue; // Don't draw number on open box
        }
        else
        {
            // Draw box number on closed box using RDPQ
            char boxNumberString[3];
            sprintf(boxNumberString, "%d", box->boxNumber);

            int textX = box->posX;
            int textY = box->posY + TEXT_BOX_OFFSET_Y;

            if (box->boxNumber < 10)
            {
                textX = box->posX + TEXT_BOX_OFFSET_X_ONE_DIGIT;
            }
            else
            {
                textX = box->posX + TEXT_BOX_OFFSET_X_TWO_DIGITS;
            }

            // Add explicit style selector
            rdpq_text_printf(NULL, FONT_SIZE_BODY, textX, textY, "^00%s", boxNumberString);
        }
    }

    // Draw cursor using RDPQ
    if (pScreenState->cursorIndex >= 0 && pScreenState->cursorIndex < NUM_BOXES_PLAYING)
    {
        Box *box_under_cursor = &pScreenState->boxes[pScreenState->cursorIndex];
        int cursorX = box_under_cursor->posX + CURSOR_BOX_OFFSET_X;
        int cursorY = box_under_cursor->posY + CURSOR_BOX_OFFSET_Y;
        rdpq_sprite_blit(cursor, cursorX, cursorY, NULL);
    }
}

void view_playing_input(ScreenPlayingState *pScreenState, struct controller_data keys)
{
    int cursorIndex = pScreenState->cursorIndex;
    int prevCursorIndex = cursorIndex;

    // Use constants defined in screen_playing.h
    const int numRows = VIEW_PLAYING_NUM_ROWS;
    const int numCols = VIEW_PLAYING_NUM_COLS;

    const int currentRow = cursorIndex / numCols;
    const int currentColumn = cursorIndex % numCols;

    // Handle directional movement
    if (keys.c[0].up)
    {
        if (currentRow > 0)
        {
            cursorIndex -= numCols;
        }
        else
        {
            // Wrap to bottom row
            cursorIndex = (numRows - 1) * numCols + currentColumn;
        }
    }
    else if (keys.c[0].down)
    {
        if (currentRow < numRows - 1)
        {
            cursorIndex += numCols;
        }
        else
        {
            // Wrap to top row
             cursorIndex = currentColumn;
        }
    }
    else if (keys.c[0].left)
    {
        if (currentColumn > 0)
        {
            cursorIndex -= 1;
        }
        else
        {
            // Wrap to rightmost column
            cursorIndex = currentRow * numCols + (numCols - 1);
        }
    }
    else if (keys.c[0].right)
    {
        if (currentColumn < numCols - 1)
        {
            cursorIndex += 1;
        }
        else
        {
            // Wrap to leftmost column
             cursorIndex = currentRow * numCols;
        }
    }
    else if (keys.c[0].A)
    {
         // Only open closed boxes - prevent reopening already opened boxes
         if (pScreenState->cursorIndex >= 0 && 
             pScreenState->cursorIndex < NUM_BOXES_PLAYING && 
             !pScreenState->boxes[pScreenState->cursorIndex].isOpen) {
                
            Box *box = &pScreenState->boxes[pScreenState->cursorIndex];
            box->isOpen = true;
            pScreenState->lastOpenedBoxNumber = box->boxNumber;
            pScreenState->openedBoxesCurrentRound++;
            pScreenState->totalBoxesOpened++;
         }
    }

    pScreenState->cursorIndex = cursorIndex;
}
