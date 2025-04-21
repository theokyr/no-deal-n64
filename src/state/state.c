#include "state.h"

void state_create_current_screen(State *pState)
{
    switch (pState->currentScreen)
    {
    case ScreenMenu:
        pState->screenMenuState = screen_menu_create();
        break;
    case ScreenPlaying:
        pState->screenPlayingState = screen_playing_create();
        break;
    case ScreenHighScores:
        pState->screenHighScoresState = screen_high_scores_create();
        break;
    case ScreenSettings:
        pState->screenSettingsState = screen_settings_create();
        break;
    default:
        break;
    }
    state_init_current_screen(pState);
}

void state_init_current_screen(State *pState)
{
    switch (pState->currentScreen)
    {
    case ScreenMenu:
        screen_menu_init(pState->screenMenuState);
        break;
    case ScreenPlaying:
        screen_playing_init(pState->screenPlayingState);
        break;
    case ScreenHighScores:
        screen_high_scores_init(pState->screenHighScoresState);
        break;
    case ScreenSettings:
        screen_settings_init(pState->screenSettingsState);
        break;
    default:
        break;
    }
}

void state_destroy_current_screen(State *pState)
{
    switch (pState->currentScreen)
    {
    case ScreenMenu:
        if (pState->screenMenuState) screen_menu_destroy(pState->screenMenuState);
        pState->screenMenuState = NULL;
        break;
    case ScreenPlaying:
        if (pState->screenPlayingState) screen_playing_destroy(pState->screenPlayingState);
        pState->screenPlayingState = NULL;
        break;
    case ScreenHighScores:
        if (pState->screenHighScoresState) screen_high_scores_destroy(pState->screenHighScoresState);
        pState->screenHighScoresState = NULL;
        break;
    case ScreenSettings:
        screen_settings_destroy(pState->screenSettingsState);
        break;
    default:
        break;
    }
}

void state_change_screen(State *pState, Screen pNewScreen)
{
    state_destroy_current_screen(pState);
    pState->currentScreen = pNewScreen;
    state_create_current_screen(pState);
}

State *state_create()
{
    State *state = (State *)malloc(sizeof(*state));
    if (!state) return NULL; // Allocation check

    // Initialize all screen state pointers to NULL
    state->screenMenuState = NULL;
    state->screenPlayingState = NULL;
    state->screenHighScoresState = NULL;
    state->screenSettingsState = NULL;

    // Set initial screen and create/init its state
    state->currentScreen = ScreenMenu; // Start with menu
    state_create_current_screen(state);
    return state;
}

void update_screen_menu_state()
{
}

void update_screen_playing_state()
{
}

void update_screen_high_scores_state()
{
}

void update_screen_settings_state()
{
}
