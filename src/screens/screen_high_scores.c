#include "screen_high_scores.h"
#include "../game.h"
#include "../screens/screen.h"
#include <rdpq.h>
#include "../fonts.h"

// Define font ID (assuming it's 1 as set in game.c)
// #define FONT_PACIFICO 1

ScreenHighScoresState *screen_high_scores_create()
{
    ScreenHighScoresState *screen = (ScreenHighScoresState *)malloc(sizeof(*screen));
    // No state needed currently
    return screen;
}

void screen_high_scores_init(ScreenHighScoresState *pScreenState)
{
    // Nothing to init
}

void screen_high_scores_destroy(ScreenHighScoresState *pScreenState)
{
    free(pScreenState);
}

void screen_high_scores_update(ScreenHighScoresState *pScreenState)
{
    // Nothing to update
}

void screen_high_scores_draw(ScreenHighScoresState *pScreenState)
{
    // Screen is cleared by rdpq_attach_clear in main loop
    // graphics_fill_screen(disp, 0x00000000);
    
    // Text color is handled by font style (set in game_init)
    // graphics_set_color(0xFFFFFF, 0x00000000);
    
    /* Draw placeholder text using RDPQ */
    rdpq_text_printf(NULL, FONT_SIZE_H2, 100, 100, "^00High Scores");
    rdpq_text_printf(NULL, FONT_SIZE_BODY, 70, 140, "^00(Not Implemented)");
    rdpq_text_printf(NULL, FONT_SIZE_BODY, 60, 180, "^00Press B to return");
}

void screen_high_scores_input(ScreenHighScoresState *pScreenState, struct controller_data keys)
{
    if (keys.c[0].B)
    {
        game_change_screen(ScreenMenu);
    }
}
