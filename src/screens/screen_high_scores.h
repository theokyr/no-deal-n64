#ifndef SCREEN_HIGHSCORES_H
#define SCREEN_HIGHSCORES_H
#include <stdlib.h>
#include <libdragon.h>

// State struct can be empty for now
typedef struct
{
} ScreenHighScoresState;

ScreenHighScoresState *screen_high_scores_create();
void screen_high_scores_init(ScreenHighScoresState *pScreenState);
void screen_high_scores_destroy(ScreenHighScoresState *pScreenState);
void screen_high_scores_update(ScreenHighScoresState *pScreenState);
void screen_high_scores_draw(ScreenHighScoresState *pScreenState);
void screen_high_scores_input(ScreenHighScoresState *pScreenState, struct controller_data keys);
#endif
