#include <stdlib.h>
#include <string.h>
#include <math.h> // For banker calculation if needed
#include <stdbool.h>
#include <rdpq.h> // Include RDPQ

#include "screen_playing.h"
#include "../rendering.h"
#include "../state/box.h"
#include "../game.h"
#include "views/playing/view_starting_box_selection.h" // Include view header
#include "views/playing/view_playing.h"                // Include view header
#include "../fonts.h" // Include font definitions
// #include "../game.h" // Needed for game_change_screen

// Define font ID (assuming it's 1 as set in game.c)
// #define FONT_PACIFICO 1 // Removed, using fonts.h

// Helper function prototypes (implementation maybe later or integrated)
static void screen_playing_change_phase(ScreenPlayingState *pScreenState, PlayingPhase newPhase);
static void screen_playing_setup_round(ScreenPlayingState *pScreenState);
static void screen_playing_prepare_playing_boxes(ScreenPlayingState *pScreenState);
static long long calculate_banker_offer(ScreenPlayingState *pState); // Added Banker Offer Calc
// Cursor validation helpers (moved from view_*.c files)
static bool is_starting_box_cursor_valid(int cursorIndex);
static bool is_playing_box_cursor_valid(int cursorIndex);

ScreenPlayingState *screen_playing_create()
{
    ScreenPlayingState *screen = (ScreenPlayingState *)malloc(sizeof(*screen));
    if (!screen)
        return NULL; // Allocation check

    // Initialize fields
    screen->boxes = NULL; // Generated in init
    screen->playerBoxIndex = -1;
    // screen->playerBox is uninitialized until selected
    screen->cursorIndex = 0;
    screen->lastOpenedBoxNumber = -1;
    screen->openedBoxesCurrentRound = 0;
    screen->totalBoxesOpened = 0;
    screen->currentRound = 0;         // Will be set to 1 in init
    screen->boxesToOpenThisRound = 0; // Set in setup_round
    screen->bankerOffer = 0;
    screen->playerBoxIsSet = false;
    screen->revealedBoxIndex = -1;           // Initialize revealedBoxIndex
    screen->dealWasAccepted = false;         // Initialize dealWasAccepted
    screen->bankerOfferDelay = 0;            // Initialize banker offer delay
    screen->noDealConfirmNeeded = false;     // Initialize no deal confirmation flag
    screen->currentPhase = Phase_NotStarted; // Will be set in init

    // Initialize sprite pointers
    screen->box_sealed_sprite = NULL;
    screen->box_unsealed_sprite = NULL;
    screen->cursor_sprite = NULL;

    return screen;
}

void screen_playing_init(ScreenPlayingState *pScreenState)
{
    if (pScreenState == NULL)
    {
        return;
    }

    // Load sprites with sprite_load
    pScreenState->box_sealed_sprite = sprite_load("rom://box_sealed.sprite");
    pScreenState->box_unsealed_sprite = sprite_load("rom://box_unsealed.sprite");
    pScreenState->cursor_sprite = sprite_load("rom://cursor.sprite");

    // Check if sprites loaded successfully
    if (!pScreenState->box_sealed_sprite || !pScreenState->box_unsealed_sprite || !pScreenState->cursor_sprite)
    {
        debugf("Failed to load one or more sprites\n");
        pScreenState->currentPhase = Phase_GameOver;
        return;
    }

    // Generate initial set of boxes
    pScreenState->boxes = boxes_generate(BOX_SPRITE_WIDTH, BOX_SPRITE_HEIGHT, VIEW_SELECTION_NUM_ROWS, VIEW_SELECTION_NUM_COLS, VIEW_SELECTION_GAP);
    if (!pScreenState->boxes)
    {
        pScreenState->currentPhase = Phase_GameOver;
        return;
    }

    // Initial state setup
    pScreenState->playerBoxIndex = -1;
    pScreenState->playerBoxIsSet = false;
    pScreenState->cursorIndex = 0;
    pScreenState->lastOpenedBoxNumber = -1;
    pScreenState->openedBoxesCurrentRound = 0;
    pScreenState->totalBoxesOpened = 0;
    pScreenState->currentRound = 0;
    pScreenState->boxesToOpenThisRound = 0;
    pScreenState->bankerOffer = 0;

    // Start the first phase: Starting Box Selection
    screen_playing_change_phase(pScreenState, Phase_StartingBoxSelection);
}

void screen_playing_destroy(ScreenPlayingState *pScreenState)
{
    if (pScreenState == NULL)
        return;

    // Free sprites properly
    if (pScreenState->box_sealed_sprite)
        sprite_free(pScreenState->box_sealed_sprite);
    if (pScreenState->box_unsealed_sprite)
        sprite_free(pScreenState->box_unsealed_sprite);
    if (pScreenState->cursor_sprite)
        sprite_free(pScreenState->cursor_sprite);

    // Free the boxes array without trying to free the prize name strings
    if (pScreenState->boxes)
    {
        // Free the array itself
        free(pScreenState->boxes);
        pScreenState->boxes = NULL;
    }

    // Free the state struct itself
    free(pScreenState);
}

void screen_playing_update(ScreenPlayingState *pScreenState)
{
    if (!pScreenState)
        return;
    // Update logic will depend on the current phase
    switch (pScreenState->currentPhase)
    {
    case Phase_StartingBoxSelection:
        // Any animations or timed events for this phase?
        break;
    case Phase_Playing:
        // Check if round goal met, trigger phase change?
        break;
    case Phase_BoxReveal:
        // Animate reveal? Timer before moving to next state?
        break;
    case Phase_BankerOffer:
        // Decrement the delay counter if it's still active
        if (pScreenState->bankerOfferDelay > 0)
        {
            pScreenState->bankerOfferDelay--;
        }
        break;
    // Add other phases
    default:
        break;
    }
}

void screen_playing_draw(ScreenPlayingState *pScreenState)
{
    // Screen is cleared by rdpq_attach_clear in main loop
    // graphics_fill_screen(disp, 0x00000000);

    // Text color is handled by font style (set in game_init)
    // graphics_set_color(0xFFFFFF, 0x00000000);

    if (pScreenState == NULL)
    {   // Use debug font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 35, 10, "^00FATAL: NULL SCREEN STATE.");
        return;
    }

    // Check if sprites are loaded before trying to draw
    if (!pScreenState->box_sealed_sprite || !pScreenState->box_unsealed_sprite || !pScreenState->cursor_sprite)
    {   // Use debug font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 35, 30, "^00ERROR: Sprites not loaded.");
        return;
    }

    // Delegate drawing based on phase
    switch (pScreenState->currentPhase)
    {
    case Phase_StartingBoxSelection:
        // Pass necessary sprites to view function
        view_starting_box_selection_draw(pScreenState, pScreenState->box_unsealed_sprite, pScreenState->box_sealed_sprite, pScreenState->cursor_sprite);
        break;
    case Phase_Playing:
        // Pass necessary sprites to view function
        view_playing_draw(pScreenState, pScreenState->box_unsealed_sprite, pScreenState->box_sealed_sprite, pScreenState->cursor_sprite);
        break;
    case Phase_BoxReveal:
        // Draw only the revealed box during reveal phase
        if (pScreenState->revealedBoxIndex >= 0 && pScreenState->revealedBoxIndex < NUM_BOXES_PLAYING)
        {
            // graphics_fill_screen(disp, 0x00000000); // Already cleared
            Box *revealedBox = &pScreenState->boxes[pScreenState->revealedBoxIndex];

            // Draw the box large in the center
            int centerX = SCREEN_WIDTH / 2 - BOX_SPRITE_WIDTH;   // Adjust positioning as needed
            int centerY = SCREEN_HEIGHT / 2 - BOX_SPRITE_HEIGHT; // Adjust positioning as needed

            // Draw the opened box using RDPQ
            if (pScreenState->box_unsealed_sprite)
            {
                rdpq_sprite_blit(pScreenState->box_unsealed_sprite, centerX, centerY, NULL);
            }

            // Draw the prize text below it using RDPQ
            if (revealedBox->prizeDisplayName)
            {
                int textX = SCREEN_WIDTH / 2 - 40; // Center text
                int textY = centerY + BOX_SPRITE_HEIGHT + 20;
                char prizeStr[50];
                sprintf(prizeStr, "%s", revealedBox->prizeDisplayName);
                // Use body font size and add explicit style selector
                rdpq_text_printf(NULL, FONT_SIZE_BODY, textX, textY, "^00%s", prizeStr);
            }

            // Draw box number using RDPQ
            char boxNumStr[5];
            sprintf(boxNumStr, "#%d", revealedBox->boxNumber);
            // Use body font size and add explicit style selector
            rdpq_text_printf(NULL, FONT_SIZE_BODY, centerX, centerY - 20, "^00%s", boxNumStr);
        }
        break;
    case Phase_BankerOffer:
        // Draw banker offer screen elements using RDPQ
        // Use H2 font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_H2, 85, 55, "^00BANKER OFFER");

        // Only show the prompt after the delay
        if (pScreenState->bankerOfferDelay <= 0)
        {
            char offerString[20];
            format_prize_display_name(offerString, sizeof(offerString), pScreenState->bankerOffer);
            // Use body font size and add explicit style selector
            rdpq_text_printf(NULL, FONT_SIZE_BODY, 100, 100, "^00%s", offerString);

            if (pScreenState->noDealConfirmNeeded)
            {
                rdpq_text_printf(NULL, FONT_SIZE_BODY, 35, 150, "^00Press B again to confirm NO DEAL");
            }
            else
            {
                rdpq_text_printf(NULL, FONT_SIZE_BODY, 35, 150, "^00Deal (START) or No Deal (B)?");
            }
        }
        else
        {
            // Maybe show a "Banker is thinking..." message during the delay
            rdpq_text_printf(NULL, FONT_SIZE_BODY, 65, 150, "^00Banker is thinking...");
        }
        break;
    case Phase_FinalBoxReveal:
        // Use H2 font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_H2, 80, 55, "^00REVEALING YOUR BOX...");
        // Draw the player's box sprite (e.g., unsealed) using RDPQ
        if (pScreenState->box_unsealed_sprite && pScreenState->playerBoxIsSet)
        {
            int boxX = (SCREEN_WIDTH - BOX_SPRITE_WIDTH) / 2;
            int boxY = 90;
            rdpq_sprite_blit(pScreenState->box_unsealed_sprite, boxX, boxY, NULL);
            // Draw the prize value inside/below using RDPQ
            if (pScreenState->playerBox.prizeDisplayName)
            {
                rdpq_text_printf(NULL, FONT_SIZE_BODY, boxX - 10, boxY + BOX_SPRITE_HEIGHT + 10, "^00%s", pScreenState->playerBox.prizeDisplayName);
            }
        }
        // Use body font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 70, 160, "^00Press A/Start to continue");
        break;
    case Phase_GameOver:
        // Use H1 font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_H1, 100, 80, "^00GAME OVER");
        char resultString[50];
        char formattedValue[12];

        // Use the reliable flag
        if (pScreenState->dealWasAccepted)
        {
            format_prize_display_name(formattedValue, sizeof(formattedValue), pScreenState->bankerOffer);
            sprintf(resultString, "You accepted the offer: %s", formattedValue);
        }
        else if (pScreenState->playerBoxIsSet)
        {
            if (pScreenState->playerBox.prizeDisplayName)
            {
                sprintf(resultString, "Your box contained: %s", pScreenState->playerBox.prizeDisplayName);
            }
            else
            {
                format_prize_display_name(formattedValue, sizeof(formattedValue), pScreenState->playerBox.prizeValue);
                sprintf(resultString, "Your box contained: %s", formattedValue);
            }
        }
        else
        {
            sprintf(resultString, "Game finished.");
        }
        // Use body font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 50, 120, "^00%s", resultString);
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 70, 160, "^00Press Start/B to return");
        break;
    // Add other phases (Phase_BoxReveal, Phase_DealAccepted, Phase_FinalBoxReveal...)
    default:
        // Draw placeholder text or handle unexpected phase?
        // Use debug font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_DEBUG, 35, 50, "^00Unhandled Playing Phase");
        break;
    }

    // Draw common UI elements using RDPQ
    char roundInfo[40];
    if (pScreenState->currentPhase == Phase_Playing)
    {
        sprintf(roundInfo, "Round: %d Boxes To Open: %d", pScreenState->currentRound, pScreenState->boxesToOpenThisRound - pScreenState->openedBoxesCurrentRound);
        // Use body font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 10, 220, "^00%s", roundInfo);
    }
    // Maybe draw player's box number permanently?
    if (pScreenState->playerBoxIsSet)
    {
        char playerBoxStr[15];
        sprintf(playerBoxStr, "Your Box: %d", pScreenState->playerBox.boxNumber);
        // Use body font size and add explicit style selector
        rdpq_text_printf(NULL, FONT_SIZE_BODY, 230, 10, "^00%s", playerBoxStr);
    }
}

void screen_playing_input(ScreenPlayingState *pScreenState, struct controller_data keys)
{
    if (pScreenState == NULL)
        return;

    // Delegate input handling based on phase
    switch (pScreenState->currentPhase)
    {
    case Phase_StartingBoxSelection:
        // No changes needed here
        view_starting_box_selection_input(pScreenState, keys);
        if (keys.c[0].start && pScreenState->playerBoxIndex >= 0)
        {
            screen_playing_change_phase(pScreenState, Phase_Playing);
        }
        break;
    case Phase_Playing:
        // Store lastOpenedBoxNumber before input to see if it changes
        int lastOpenedBoxBefore = pScreenState->lastOpenedBoxNumber;
        int openedBoxesCountBefore = pScreenState->openedBoxesCurrentRound;

        view_playing_input(pScreenState, keys);

        // Check if a box was newly opened by seeing if the opened boxes count increased
        if (pScreenState->openedBoxesCurrentRound > openedBoxesCountBefore)
        {
            pScreenState->revealedBoxIndex = pScreenState->cursorIndex;
            screen_playing_change_phase(pScreenState, Phase_BoxReveal);
        }
        break;
    case Phase_BoxReveal:
        // Wait 2 seconds, then auto-transition back to playing or banker offer
        // This uses a timer approach - in real code, you'd implement a timer
        static int reveal_counter = 0;
        reveal_counter++;

        // Auto-transition after brief delay (or with key press)
        if (reveal_counter > 60 || keys.c[0].A || keys.c[0].start)
        {
            reveal_counter = 0;
            pScreenState->revealedBoxIndex = -1;

            // Check if round is over
            if (pScreenState->openedBoxesCurrentRound >= pScreenState->boxesToOpenThisRound)
            {
                // Check if it's the very last box before the final reveal
                if (pScreenState->totalBoxesOpened >= NUM_BOXES_PLAYING - 1)
                {
                    screen_playing_change_phase(pScreenState, Phase_FinalBoxReveal);
                }
                else
                {
                    screen_playing_change_phase(pScreenState, Phase_BankerOffer);
                }
            }
            else
            {
                // Round is not over, go back to playing
                screen_playing_change_phase(pScreenState, Phase_Playing);
            }
        }
        break;
    case Phase_BankerOffer:
        // Only process inputs after the delay has expired
        if (pScreenState->bankerOfferDelay <= 0)
        {
            if (pScreenState->noDealConfirmNeeded)
            {
                // No Deal confirmation required
                if (keys.c[0].B)
                {
                    // Confirmed No Deal - move to next round
                    pScreenState->currentRound++;
                    if (pScreenState->currentRound > MAX_ROUND)
                    {
                        screen_playing_change_phase(pScreenState, Phase_FinalBoxReveal);
                    }
                    else
                    {
                        screen_playing_change_phase(pScreenState, Phase_Playing); // Start next round
                    }
                }
                else if (keys.c[0].A || keys.c[0].start)
                {
                    // Canceled No Deal with A or Start
                    pScreenState->noDealConfirmNeeded = false;
                }
            }
            else
            {
                // Initial choice - Deal or No Deal
                if (keys.c[0].start)
                { // Deal - changed from A to start
                    screen_playing_change_phase(pScreenState, Phase_DealAccepted);
                }
                else if (keys.c[0].B)
                {                                             // First No Deal press
                    pScreenState->noDealConfirmNeeded = true; // Set confirmation needed
                }
            }
        }
        break;
    case Phase_FinalBoxReveal:
        // Wait for player confirmation before moving to game over
        if (keys.c[0].A || keys.c[0].start)
        {
            screen_playing_change_phase(pScreenState, Phase_GameOver);
        }
        break;
    case Phase_DealAccepted:
        // Set the flag
        pScreenState->dealWasAccepted = true;
        // Go directly to GameOver without recursion
        pScreenState->currentPhase = Phase_GameOver;
        break;
    case Phase_GameOver:
        if (keys.c[0].start || keys.c[0].B)
        {
            game_change_screen(ScreenMenu); // Transition back to menu
        }
        break;
    // Add other phases
    default:
        break;
    }
}

// --- Helper Functions ---

// Function to change the internal phase and perform setup for the new phase
static void screen_playing_change_phase(ScreenPlayingState *pScreenState, PlayingPhase newPhase)
{
    if (!pScreenState)
        return;

    PlayingPhase oldPhase = pScreenState->currentPhase;
    pScreenState->currentPhase = newPhase;

    // Reset flags unless explicitly set by the new phase
    if (newPhase != Phase_DealAccepted)
    {
        pScreenState->dealWasAccepted = false;
    }

    // Store cursor position before phase change if needed
    int prevCursorIndex = pScreenState->cursorIndex;

    // Setup for the new phase
    switch (newPhase)
    {
    case Phase_StartingBoxSelection:
        pScreenState->currentRound = 1;
        boxes_recalculate_positions(pScreenState->boxes, BOX_SPRITE_WIDTH, BOX_SPRITE_HEIGHT, VIEW_SELECTION_NUM_ROWS, VIEW_SELECTION_NUM_COLS, VIEW_SELECTION_GAP);
        pScreenState->cursorIndex = 0; // Reset for initial selection
        break;
    case Phase_Playing:
        // Reset revealed index when entering playing phase
        pScreenState->revealedBoxIndex = -1;
        // If transitioning from selection, prepare the playing boxes array
        if (!pScreenState->playerBoxIsSet)
        {
            screen_playing_prepare_playing_boxes(pScreenState);
            pScreenState->playerBoxIsSet = true;
            pScreenState->cursorIndex = 0; // Reset cursor only on first entry
            boxes_recalculate_positions(pScreenState->boxes, BOX_SPRITE_WIDTH, BOX_SPRITE_HEIGHT, VIEW_PLAYING_NUM_ROWS, VIEW_PLAYING_NUM_COLS, VIEW_PLAYING_GAP);
        }
        else if (oldPhase == Phase_BoxReveal)
        {
            // Keep the cursor where it was after revealing a box
            pScreenState->cursorIndex = prevCursorIndex;
        }
        // Setup round variables
        if (oldPhase != Phase_BoxReveal)
        {
            screen_playing_setup_round(pScreenState);
        }
        break;
    case Phase_BankerOffer:
        // Calculate banker offer (Needs implementation)
        pScreenState->bankerOffer = calculate_banker_offer(pScreenState);
        // Initialize the delay counter for the prompt (60 frames = ~1 second)
        pScreenState->bankerOfferDelay = 120;      // ~2 seconds delay
        pScreenState->noDealConfirmNeeded = false; // Reset confirmation flag
        break;
    case Phase_DealAccepted:
        // Set the flag
        pScreenState->dealWasAccepted = true;
        // Go directly to GameOver without recursion
        pScreenState->currentPhase = Phase_GameOver;
        break;
    case Phase_FinalBoxReveal:
        pScreenState->bankerOffer = 0;
        break;
    case Phase_GameOver:
        // Final state setup (nothing specific needed here now)
        break;
    // Add other phase setups
    default:
        break;
    }
}

// Determines how many boxes to open for the current round
static int get_boxes_for_round(int round)
{
    if (round == 1)
        return BOXES_ROUND_1;
    if (round == 2)
        return BOXES_ROUND_2;
    if (round == 3)
        return BOXES_ROUND_3;
    if (round == 4)
        return BOXES_ROUND_4;
    if (round == 5)
        return BOXES_ROUND_5;
    if (round >= 6 && round <= MAX_ROUND)
        return BOXES_ROUND_6_TO_10;
    return 0; // Should not happen in a normal game flow
}

// Sets up variables for the current playing round
static void screen_playing_setup_round(ScreenPlayingState *pScreenState)
{
    if (!pScreenState)
        return;
    pScreenState->boxesToOpenThisRound = get_boxes_for_round(pScreenState->currentRound);
    pScreenState->openedBoxesCurrentRound = 0; // Reset counter for the new round
    // Any other round setup needed?
}

// Updated prepare_playing_boxes for correct string memory management
static void screen_playing_prepare_playing_boxes(ScreenPlayingState *pScreenState)
{
    if (!pScreenState || pScreenState->playerBoxIndex < 0 || pScreenState->playerBoxIndex >= NUM_BOXES)
        return;

    Box *allBoxes = pScreenState->boxes; // The original array of NUM_BOXES
    Box *playingBoxes = (Box *)malloc(NUM_BOXES_PLAYING * sizeof(Box));
    if (!playingBoxes)
    {
        pScreenState->currentPhase = Phase_GameOver;
        // Free display names from allBoxes before freeing the array
        if (allBoxes)
        {
            for (int i = 0; i < NUM_BOXES; ++i)
            {
                if (allBoxes[i].prizeDisplayName)
                    free((void *)allBoxes[i].prizeDisplayName);
            }
            free(allBoxes);
        }
        pScreenState->boxes = NULL;
        return;
    }

    int newBoxIndex = 0;

    for (int i = 0; i < NUM_BOXES; i++)
    {
        if (i != pScreenState->playerBoxIndex)
        {
            if (newBoxIndex < NUM_BOXES_PLAYING)
            {
                playingBoxes[newBoxIndex] = allBoxes[i];
                allBoxes[i].prizeDisplayName = NULL;
                newBoxIndex++;
            }
            else
            {
                // Free the string if it wasn't the static error string (just check non-NULL)
                if (allBoxes[i].prizeDisplayName)
                {
                    free((void *)allBoxes[i].prizeDisplayName);
                }
                allBoxes[i].prizeDisplayName = NULL;
            }
        }
    }

    pScreenState->playerBox = allBoxes[pScreenState->playerBoxIndex];
    allBoxes[pScreenState->playerBoxIndex].prizeDisplayName = NULL;

    for (int i = 0; i < NUM_BOXES; ++i)
    {
        // Free the string if it wasn't the static error string (just check non-NULL)
        if (allBoxes[i].prizeDisplayName)
        {
            free((void *)allBoxes[i].prizeDisplayName);
        }
    }

    free(allBoxes);
    pScreenState->boxes = playingBoxes;
}

// Banker Offer Calculation - Refined
static long long calculate_banker_offer(ScreenPlayingState *pState)
{
    if (!pState || !pState->boxes)
        return 0;

    long long sum_remaining = 0;
    int count_remaining = 0;

    // Sum values of unopened boxes currently in play
    for (int i = 0; i < NUM_BOXES_PLAYING; ++i)
    {
        // Ensure box is not open AND prizeValue > 0 (ignore trash items for calculation)
        if (!pState->boxes[i].isOpen && pState->boxes[i].prizeValue > 0)
        {
            sum_remaining += pState->boxes[i].prizeValue;
            count_remaining++;
        }
    }

    // Add player's box value (it's always unopened at this stage)
    if (pState->playerBoxIsSet && pState->playerBox.prizeValue > 0)
    { // Check prizeValue > 0
        sum_remaining += pState->playerBox.prizeValue;
        count_remaining++;
    }

    // Avoid division by zero if only trash boxes ($0) are left
    if (count_remaining == 0)
        return 1; // Offer $1 as a minimum if only trash remains

    // Calculate Expected Value (Average)
    double expected_value = (double)sum_remaining / count_remaining;

    // Apply Banker's Formula:
    // Expected Value * (Round / Max Rounds)^Exponent * RiskFactor
    // Using Exponent=2 makes offers climb faster later in the game.
    // RiskFactor < 1 makes the banker offer less than the pure average.
    double round_factor = pow((double)pState->currentRound / MAX_ROUND, 2.0);
    double risk_factor = 0.75; // Banker takes 75% of the calculated value
    double offer = expected_value * round_factor * risk_factor;

    // Ensure the offer is at least 1
    long long final_offer = (long long)fmax(1.0, offer);

    return final_offer;
}

// Moved from view_starting_box_selection.c
static bool is_starting_box_cursor_valid(int cursorIndex)
{
    return cursorIndex >= 0 && cursorIndex < NUM_BOXES;
}

// Moved from view_playing.c
static bool is_playing_box_cursor_valid(int cursorIndex)
{
    return cursorIndex >= 0 && cursorIndex < NUM_BOXES_PLAYING;
}
