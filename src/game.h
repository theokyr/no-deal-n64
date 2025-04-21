#ifndef GAME_H
#define GAME_H
#include <libdragon.h>
#include "state/state.h"
// #include "state/playing_state.h" // Removed
#include "screens/screen_menu.h"
#include "screens/screen_playing.h"
// #include "screens/screen_playing_banker.h" // Removed
#include "screens/screen_high_scores.h"
#include "screens/screen_settings.h"

void game_init();
void game_init_playing_state();
void game_init_current_screen();
void game_change_screen(Screen pScreen);
void game_destroy_current_screen();
void game_update();
void game_draw();
void game_update_input();
void game_onNewGameStarted();
#endif
