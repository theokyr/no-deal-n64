#ifndef STATE_H
#define STATE_H
#include <stdlib.h>
// #include "playing_state.h" // Removed
#include "../screens/screen.h"
#include "../screens/screen_menu.h"
#include "../screens/screen_playing.h"
// #include "../screens/screen_playing_banker.h" // Removed
// #include "../screens/screen_game_over.h" // Removed
#include "../screens/screen_high_scores.h"
#include "../screens/screen_settings.h"

typedef struct
{
    Screen currentScreen;
    ScreenMenuState *screenMenuState;
    ScreenPlayingState *screenPlayingState;
    // ScreenPlayingBankerState *screenPlayingBankerState; // Removed
    // ScreenGameOverState *screenGameOverState; // Removed
    ScreenHighScoresState *screenHighScoresState;
    ScreenSettingsState *screenSettingsState;
} State;

State *state_create();
void state_change_screen(State *pState, Screen pNewScreen);
void state_create_current_screen(State *pState);
void state_init_current_screen(State *pState);
void state_destroy_current_screen(State *pState);
void update_screen_menu_state();
void update_screen_playing_state();
void update_screen_game_over_state();
void update_screen_high_scores_state();
void update_screen_settings_state();
#endif
