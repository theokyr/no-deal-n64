#ifndef VIEW_STARTING_BOX_SELECTION_H
#define VIEW_STARTING_BOX_SELECTION_H
#include <stdlib.h>
#include <libdragon.h>

// #include "../../../state/playing_state.h" // No longer needed
#include "../../../screens/screen_playing.h" // Include the new state definition
#include "../../../state/box.h"

// Forward declare ScreenPlayingState
struct ScreenPlayingState;

#define CURSOR_BOX_OFFSET_X 15
#define CURSOR_BOX_OFFSET_Y 10

#define TEXT_BOX_OFFSET_X_ONE_DIGIT 9
#define TEXT_BOX_OFFSET_X_TWO_DIGITS 6
#define TEXT_BOX_OFFSET_Y 4

#define STARTING_BOX_SELECTION_LAST_ROW_UP_SHIFT 2
#define STARTING_BOX_SELECTION_LAST_ROW_LEFT_SHIFT 3

void view_starting_box_selection_draw(struct ScreenPlayingState *pScreenState, sprite_t *box_unsealed, sprite_t *box_sealed, sprite_t *cursor);
void view_starting_box_selection_input(struct ScreenPlayingState *pScreenState, struct controller_data keys);

#endif
