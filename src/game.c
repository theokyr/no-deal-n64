#include <libdragon.h> // Add missing include for RGBA32 etc.
#include <graphics.h>  // Include for color_t definition
#include <rdpq.h>      // Include RDPQ
#include <rdpq_font.h> // Include RDPQ Font
#include "game.h"
#include "fonts.h"

State *state;
bool debug = false;

void game_init()
{
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
    dfs_init(DFS_DEFAULT_LOCATION);
    rdpq_init();
    controller_init();
    timer_init();

    // Load and register font
    rdpq_font_t *fnt = rdpq_font_load("rom://font.font64");
    if (!fnt) {
         debugf("ERROR: Failed to load font rom://font.font64\n");
         // Handle error appropriately, maybe fallback or stop
    } else {
        // Ignore RGBA32 linter error - VS Code is incorrectly setup. Set a default style (white text for style 0)
        rdpq_font_style(fnt, 0, &(rdpq_fontstyle_t){ .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF) });
        // Ignore RGBA32 linter error - VS Code is incorrectly setup. Add style 1 for highlighting if needed
        rdpq_font_style(fnt, 1, &(rdpq_fontstyle_t){ .color = RGBA32(0xFF, 0xFF, 0x00, 0xFF) });
        rdpq_text_register_font(FONT_ID_PACIFICO, fnt);
    }

    state = state_create();
}

void game_change_screen(Screen pScreen)
{
    state_change_screen(state, pScreen);
}

void game_update()
{
    switch (state->currentScreen)
    {
    case ScreenMenu:
        screen_menu_update(state->screenMenuState);
        break;
    case ScreenPlaying:
        screen_playing_update(state->screenPlayingState);
        break;
    case ScreenHighScores:
        screen_high_scores_update(state->screenHighScoresState);
        break;
    case ScreenSettings:
        screen_settings_update(state->screenSettingsState);
        break;
    default:
        break;
    }
}

void game_draw()
{
    // Set default RDPQ modes for drawing (can be overridden by screens)
    rdpq_set_mode_standard();
    rdpq_mode_alphacompare(1); // Enable alpha testing for transparency

    switch (state->currentScreen)
    {
    case ScreenMenu:
        screen_menu_draw(state->screenMenuState);
        break;
    case ScreenPlaying:
        screen_playing_draw(state->screenPlayingState);
        break;
    case ScreenHighScores:
        screen_high_scores_draw(state->screenHighScoresState);
        break;
    case ScreenSettings:
        screen_settings_draw(state->screenSettingsState);
        break;
    default:
        break;
    }

    if (debug)
    {
        // Use RDPQ for debug text, prepending font ID escape code ^10
        if (state != NULL)
        {
            rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 10, 10, "^00Init Game State...OK");
        }
        else
        {
            rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 10, 10, "^00Init Game State...ERR");
        }

        if (state->screenMenuState != NULL)
        {
            rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 10, 20, "^00Init ScreenMenuState...OK");
        }
        else
        {
            rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 10, 20, "^00Init ScreenMenuState...ERR");
        }
    }
}

void game_update_input()
{
    controller_scan();
    struct controller_data keys = get_keys_down();
    switch (state->currentScreen)
    {
    case ScreenMenu:
        screen_menu_input(state->screenMenuState, keys);
        break;
    case ScreenPlaying:
        screen_playing_input(state->screenPlayingState, keys);
        break;
    case ScreenHighScores:
        screen_high_scores_input(state->screenHighScoresState, keys);
        break;
    case ScreenSettings:
        screen_settings_input(state->screenSettingsState, keys);
        break;
    default:
        break;
    }
}
