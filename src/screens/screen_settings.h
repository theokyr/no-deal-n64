#ifndef SCREEN_SETTINGS_H
#define SCREEN_SETTINGS_H
#include <stdlib.h>
#include <libdragon.h>

// State struct can be empty for now
typedef struct
{
} ScreenSettingsState;

ScreenSettingsState *screen_settings_create();
void screen_settings_init(ScreenSettingsState *pScreenState);
void screen_settings_destroy(ScreenSettingsState *pScreenState);
void screen_settings_update(ScreenSettingsState *pScreenState);
void screen_settings_draw(ScreenSettingsState *pScreenState);
// void screen_settings_onCheckPressedB(struct controller_data keys); // Removed
void screen_settings_input(ScreenSettingsState *pScreenState, struct controller_data keys);
#endif
