#include "screen_menu.h"
#include "../game.h"
#include <rdpq.h>      // Include RDPQ
#include "../fonts.h" // Include font definitions

sprite_t *ferentinos;

// Define font ID (assuming it's 1 as set in game.c)
// #define FONT_PACIFICO 1 // Removed, using fonts.h

ScreenMenuState *screen_menu_create()
{
    ScreenMenuState *screen = (ScreenMenuState *)malloc(sizeof(*screen));
    screen->cursorPosition = 0;
    screen->cursorPositionMin = 0;
    screen->cursorPositionMax = 2;
    return screen;
}

void screen_menu_init(ScreenMenuState *pScreenState)
{
    ferentinos = sprite_load("rom://ferentinos.sprite");
}

void screen_menu_destroy(ScreenMenuState *pScreenState)
{
    if (pScreenState) free(pScreenState);

    if (ferentinos) {
        sprite_free(ferentinos);
        ferentinos = NULL;
    }
}

void screen_menu_update(ScreenMenuState *pScreenState)
{
}

void screen_menu_draw(ScreenMenuState *pScreenState)
{
    // Screen is cleared by rdpq_attach_clear in main loop
    // graphics_fill_screen(disp, 0x00000000);

    // Assuming standard RDPQ mode is set in game_draw

    /* Display sprite using RDPQ */
    if (ferentinos) {
        rdpq_sprite_blit(ferentinos, 125, 30, NULL);
    }

    // Text color is handled by font style (set in game_init)
    // graphics_set_color(0xFFFFFF, 0x00000000);

    /* Draw text using RDPQ */
    rdpq_text_printf(NULL, FONT_SIZE_H1, 130, 100, "^00NO DEAL");
    rdpq_text_printf(NULL, FONT_SIZE_BODY, 40, 120, "^00Start Game");
    rdpq_text_printf(NULL, FONT_SIZE_BODY, 40, 140, "^00High Scores");
    rdpq_text_printf(NULL, FONT_SIZE_BODY, 40, 160, "^00Settings");

    /* Draw cursor using RDPQ */
    if (pScreenState != NULL)
    {
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 20, 120 + (20 * pScreenState->cursorPosition), "^00->");
    }
}

void screen_menu_onCheckPressedUp(ScreenMenuState *pScreenState, struct controller_data keys)
{
    if (keys.c[0].up)
    {
        if (pScreenState->cursorPosition <= pScreenState->cursorPositionMin)
        {
            pScreenState->cursorPosition = pScreenState->cursorPositionMax; // jump to bottom
            return;
        }
        pScreenState->cursorPosition--;
    }
}

void screen_menu_onCheckPressedDown(ScreenMenuState *pScreenState, struct controller_data keys)
{
    if (keys.c[0].down)
    {
        if (pScreenState->cursorPosition >= pScreenState->cursorPositionMax)
        {
            pScreenState->cursorPosition = pScreenState->cursorPositionMin; // return to start
            return;
        }
        pScreenState->cursorPosition++;
    }
}

void screen_menu_onCheckPressedA(ScreenMenuState *pScreenState, struct controller_data keys)
{
    if (keys.c[0].A)
    {
        switch (pScreenState->cursorPosition)
        {
        case 0:
            game_change_screen(ScreenPlaying);
            break;
        case 1:
            game_change_screen(ScreenHighScores);
            break;
        case 2:
            game_change_screen(ScreenSettings);
            break;
        default:
            break;
        }
    }
}

void screen_menu_input(ScreenMenuState *pScreenState, struct controller_data keys)
{
    if (pScreenState == NULL)
    {
        return;
    }

    screen_menu_onCheckPressedUp(pScreenState, keys);
    screen_menu_onCheckPressedDown(pScreenState, keys);
    screen_menu_onCheckPressedA(pScreenState, keys);
}
