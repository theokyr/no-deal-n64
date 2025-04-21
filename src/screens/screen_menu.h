#ifndef SCREEN_MENU_H
#define SCREEN_MENU_H

#include <stdlib.h>
#include <libdragon.h>

typedef struct
{
    int cursorPosition;
    int cursorPositionMin;
    int cursorPositionMax;
} ScreenMenuState;

ScreenMenuState *screen_menu_create();
void screen_menu_init(ScreenMenuState *pScreenState);
void screen_menu_destroy(ScreenMenuState *pScreenState);
void screen_menu_update(ScreenMenuState *pScreenState);
void screen_menu_draw(ScreenMenuState *pScreenState);
void screen_menu_onCheckPressedUp(ScreenMenuState *pScreenState, struct controller_data keys);
void screen_menu_onCheckPressedDown(ScreenMenuState *pScreenState, struct controller_data keys);
void screen_menu_onCheckPressedA(ScreenMenuState *pScreenState, struct controller_data keys);
void screen_menu_input(ScreenMenuState *pScreenState, struct controller_data keys);

#endif
