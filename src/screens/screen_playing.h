#ifndef SCREEN_PLAYING_H
#define SCREEN_PLAYING_H
#include <stdlib.h>
#include <stdbool.h> // Include for bool type
#include <libdragon.h>
// #include "../state/playing_state.h" // No longer needed directly
#include "../state/box.h"
// #include "views/playing/view_starting_box_selection.h" // Remove include
// #include "views/playing/view_playing.h" // Remove include

// Forward declare structs used in function signatures if needed
// struct Box;
// struct display_context_t;
// struct sprite_t;
// struct controller_data;

// --- Constants previously in playing_state.h ---
#define VIEW_SELECTION_NUM_ROWS 4
#define VIEW_SELECTION_NUM_COLS 7
#define VIEW_SELECTION_GAP 5

#define VIEW_PLAYING_NUM_ROWS 5
#define VIEW_PLAYING_NUM_COLS 5
#define VIEW_PLAYING_GAP 20

#define MIN_ROUND 1
#define MAX_ROUND 10

#define BOXES_ROUND_1 6
#define BOXES_ROUND_2 5
#define BOXES_ROUND_3 4
#define BOXES_ROUND_4 3
#define BOXES_ROUND_5 2
#define BOXES_ROUND_6_TO_10 1
// --- End Constants ---

// Define the phases within the playing screen
typedef enum
{
    Phase_NotStarted,             // Initial state before setup
    Phase_StartingBoxSelection,   // Player selects their initial box
    Phase_Playing,                // Player opens boxes round by round
    Phase_BoxReveal,              // Showing the value of an opened box (optional, can be integrated into Playing)
    Phase_BankerOffer,            // Banker makes an offer
    Phase_DealAccepted,           // Player accepted the banker's offer
    Phase_FinalBoxReveal,         // Revealing the player's box value at the end
    Phase_GameOver                // Game finished, ready to transition out
} PlayingPhase;

// Define the ScreenPlayingState struct BEFORE it's used in function signatures
typedef struct ScreenPlayingState ScreenPlayingState;
struct ScreenPlayingState
{
    // Fields merged from PlayingState
    Box *boxes;                 // Array of boxes currently in play
    int playerBoxIndex;         // Index of the box the player initially chose (-1 if none)
    Box playerBox;              // A copy of the player's chosen box's data
    int cursorIndex;            // Current cursor position on the box grid
    int lastOpenedBoxNumber;    // Box number of the most recently opened box (-1 if none)
    int openedBoxesCurrentRound; // Number of boxes opened in the *current* round
    int totalBoxesOpened;       // Total number of boxes opened in the game
    int currentRound;           // Current game round (1-10)
    int boxesToOpenThisRound;   // How many boxes the player must open in this round
    long long bankerOffer;      // Current offer from the banker
    bool playerBoxIsSet;        // Flag indicating if the player has chosen their box
    int revealedBoxIndex;       // Index of the box just revealed in Phase_BoxReveal (-1 if none)
    bool dealWasAccepted;       // Flag indicating if the player accepted the banker's deal
    int bankerOfferDelay;       // Counter for delay before showing prompt
    bool noDealConfirmNeeded;   // Flag for requiring No Deal confirmation

    PlayingPhase currentPhase; // Current phase within the playing screen

    // Sprites used by this screen
    sprite_t *box_sealed_sprite;
    sprite_t *box_unsealed_sprite;
    sprite_t *cursor_sprite;
};

ScreenPlayingState *screen_playing_create(); // No longer needs PlayingState*
void screen_playing_init(ScreenPlayingState *pScreenState);
void screen_playing_destroy(ScreenPlayingState *pScreenState);
void screen_playing_update(ScreenPlayingState *pScreenState);
void screen_playing_draw(ScreenPlayingState *pScreenState);
void screen_playing_input(ScreenPlayingState *pScreenState, struct controller_data keys);

// Add helper function prototypes used internally by screen_playing.c if needed elsewhere?
// static void screen_playing_change_phase(ScreenPlayingState *pScreenState, PlayingPhase newPhase);
// static long long calculate_banker_offer(ScreenPlayingState *pState);

#endif
