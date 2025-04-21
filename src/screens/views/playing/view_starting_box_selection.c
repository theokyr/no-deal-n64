#include "view_starting_box_selection.h"
#include <rdpq.h> // Include RDPQ
#include "../../../fonts.h" // Include font definitions

// Define font ID (assuming it's 1 as set in game.c)
// #define FONT_PACIFICO 1 // Removed, using fonts.h

// Remove helper function, moved to screen_playing.c
/*
static bool is_starting_box_cursor_valid(int cursorIndex) {
    return cursorIndex >= 0 && cursorIndex < NUM_BOXES;
}
*/

// Use ScreenPlayingState* from the struct definition now
void view_starting_box_selection_draw(ScreenPlayingState *pScreenState, sprite_t *box_unsealed, sprite_t *box_sealed, sprite_t *cursor)
{
    // Add null checks at beginning
    if (!pScreenState || !box_sealed || !box_unsealed || !cursor) {
        // Static string
        rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 35, 30, "^00ERROR: Invalid sprites or state.");
        return;
    }

    // Static string
    rdpq_text_printf(NULL, FONT_SIZE_H2, 85, 55, "^00PLEASE SELECT A BOX");

    if (pScreenState->playerBoxIndex >= 0)
    {
        // Get the box number directly from the selected index, boxes array still has all boxes here
        Box *player_selection_box = &pScreenState->boxes[pScreenState->playerBoxIndex];

        char boxNumberString[3];
        sprintf(boxNumberString, "%d", player_selection_box->boxNumber);

        // Static string for label
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 95, 170, "^00Selected Box: ");
        // Correct: Use "%s" for box number string
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 205, 170, "^00%s", boxNumberString);
        // Static string for prompt
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 42, 190, "^00Press Start to begin the game!");
    }

    /* Draw the boxes */
    sprite_t *box_sprite;
    // Draw all NUM_BOXES in the selection phase
    for (int i = 0; i < NUM_BOXES; i++)
    {
        Box *box = &(pScreenState->boxes[i]);
        // Draw the box sprite regardless of whether it's open or not (should always be sealed here)
        if (box->isOpen)
        {
            box_sprite = box_unsealed; // Should not happen in selection
        }
        else
        {
            box_sprite = box_sealed;
        }

        // Use RDPQ for sprite drawing
        rdpq_sprite_blit(box_sprite, box->posX, box->posY, NULL);

        // Draw box number inside sealed box using RDPQ
        char boxNumberString[3]; // Redeclare locally
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
        // Correct: Use "%s" for box number string
        rdpq_text_printf(NULL, FONT_SIZE_BODY, textX, textY, "^00%s", boxNumberString);

    }

    // Draw cursor based on cursorIndex using RDPQ
    if (pScreenState->cursorIndex >= 0 && pScreenState->cursorIndex < NUM_BOXES) // Basic check for now
    {
       Box *box_under_cursor = &pScreenState->boxes[pScreenState->cursorIndex];
       int cursorX = box_under_cursor->posX + CURSOR_BOX_OFFSET_X;
       int cursorY = box_under_cursor->posY + CURSOR_BOX_OFFSET_Y;
       rdpq_sprite_blit(cursor, cursorX, cursorY, NULL);
    }
}

void view_starting_box_selection_input(ScreenPlayingState *pScreenState, struct controller_data keys)
{
    int cursorIndex = pScreenState->cursorIndex;
    int prevCursorIndex = cursorIndex;

    // Use constants defined in screen_playing.h
    const int numRows = VIEW_SELECTION_NUM_ROWS;
    const int numCols = VIEW_SELECTION_NUM_COLS;

    const int currentRow = cursorIndex / numCols;
    const int currentColumn = cursorIndex % numCols;

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
        // Select the box under the cursor
        if (cursorIndex >= 0 && cursorIndex < NUM_BOXES)
        {
             pScreenState->playerBoxIndex = cursorIndex;
        }
    }
    else if (keys.c[0].start)
    {
        // Start is handled in screen_playing_input to trigger phase change
        // Only proceed if a box has been selected.
        if (pScreenState->playerBoxIndex >= 0) {
             // The actual phase change is done in screen_playing.c
             // We might set a flag here if needed, but checking playerBoxIndex >= 0 should suffice
             // playing_state_starting_box_selection_finish(pPlayingState); // This logic moved to screen_playing_prepare_playing_boxes
             // playing_state_change_view(pPlayingState, Playing); // This logic moved to screen_playing_change_phase
        }
    }

    // Validate the new cursor index
    // Call the validation function defined in screen_playing.c?
    // No, views shouldn't know about screen internals ideally.
    // The validation should happen *after* this function returns, in screen_playing_input.
    // For now, remove the validation here.

    /*
    if (!is_starting_box_cursor_valid(cursorIndex))
    {
        cursorIndex = prevCursorIndex;
    }
    */

    pScreenState->cursorIndex = cursorIndex;
}
