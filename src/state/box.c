#include "box.h"

const char *trashNames[NUM_TRASH_NAMES] = {
    "Spoon", "Toothbrush", "Rubber band", "Keychain", "Batteries", "Candles", "Stapler",
    "USB Stick", "Tissues", "Remote control", "Coins", "Chewing gum", "Tacks", "Rubber duck", "Highlighter",
    "Ruler", "Pencil", "Bottle cap", "Eraser", "Safety pin", "Screwdriver", "Mirror", "Binder clip",
    "Paper clip", "Carrot", "Apple", "Banana", "Orange", "Grapes", "Peach", "Strawberry", "Raspberry",
    "Blueberry", "Blackberry", "Cherry", "Melon", "Watermelon", "Lime", "Lemon", "Grapefruit", "Cantaloupe",
    "Honeydew", "Pear", "Pineapple", "Apricot", "Fig", "Plum", "Kiwi", "Papaya", "Mango"};

const int prizes[] = {1, 5, 10, 50, 100, 200, 500, 750, 1000, 5000, 10000,
                      25000, 50000, 75000, 100000, 200000, 300000, 400000,
                      500000, 750000, 1000000, 1337};

// Define a static error string for strdup failures
static const char *strdup_error_string = "ERR";

void format_prize_display_name(char *buffer, int size, int prize_value)
{
    // Escape dollar sign as it's used for font selection in rdpq
    if (prize_value < 1000)
    {
        snprintf(buffer, size, "$$%d", prize_value);
    }
    else if (prize_value < 1000000)
    {
        snprintf(buffer, size, "$$%d,%03d", prize_value / 1000, prize_value % 1000);
    }
    else
    {
        snprintf(buffer, size, "$$%d,%03d,%03d", prize_value / 1000000, 
                (prize_value / 1000) % 1000, prize_value % 1000);
    }
}

void boxes_shuffle(Box *boxes)
{
    // Shuffle the boxes
    for (int i = 0; i < NUM_BOXES; i++)
    {
        int j = rand() % (NUM_BOXES - i) + i;
        Box temp = boxes[i];
        boxes[i] = boxes[j];
        boxes[j] = temp;
    }
}

void boxes_assign_numbers(Box *boxes)
{
    // Number the boxes
    for (int i = 0; i < NUM_BOXES; i++)
    {
        boxes[i].boxNumber = i + 1;
        boxes[i].isOpen = false;
    }
}

void boxes_assign_prizes(Box *boxes)
{
    int prizesIndex = 0;
    char valueBuffer[32]; // Increased buffer size to be safe

    for (int i = 0; i < NUM_BOXES; i++)
    {
        if (i < NUM_TRASH_BOXES)
        {
            boxes[i].prizeValue = 0;
            // Simply point to the static string without strdup
            boxes[i].prizeDisplayName = trashNames[i % NUM_TRASH_NAMES];
        }
        else
        {
            boxes[i].prizeValue = prizes[prizesIndex % (sizeof(prizes)/sizeof(prizes[0]))];
            // Format prize with proper commas and dollar sign
            format_prize_display_name(valueBuffer, sizeof(valueBuffer), boxes[i].prizeValue);
            boxes[i].prizeDisplayName = strdup(valueBuffer);
            
            // Fallback if strdup fails - use a static placeholder
            if (boxes[i].prizeDisplayName == NULL) {
                boxes[i].prizeDisplayName = "$ERROR";
            }
            
            prizesIndex++;
        }
    }
}

Box *boxes_generate(int boxWidth, int boxHeight, int numRows, int numCols, int gap)
{
    Box *boxes = (Box *)malloc(NUM_BOXES * sizeof(Box));
    if (!boxes) return NULL; // Allocation check

    // Initialize basic fields before assigning prizes/numbers
    for(int i=0; i < NUM_BOXES; ++i) {
        boxes[i].isOpen = false;
        boxes[i].posX = 0;
        boxes[i].posY = 0;
        boxes[i].prizeValue = -1; // Indicate unset
        boxes[i].prizeDisplayName = NULL; // Initialize to NULL
        boxes[i].boxNumber = -1;
    }

    boxes_assign_prizes(boxes);
    boxes_shuffle(boxes);
    boxes_assign_numbers(boxes);

    // Note: Initial positions are calculated later by screen_playing_change_phase

    return boxes;
}

void boxes_recalculate_positions(Box *boxes, int boxWidth, int boxHeight, int numRows, int numCols, int gap)
{
    // Calculate the total screen width and height
    int totalWidth = numCols * (boxWidth + gap) - gap;
    int totalHeight = numRows * (boxHeight + gap) - gap;

    // Calculate the left and top offsets to center the boxes on the screen
    int leftOffset = (SCREEN_WIDTH - totalWidth) / 2;
    int topOffset = (SCREEN_HEIGHT - totalHeight) / 2;

    // Assign positions
    int currentBox = 0;
    for (int row = 0; row < numRows; row++)
    {

        for (int col = 0; col < numCols; col++)
        {
            boxes[currentBox].posX = leftOffset + col * (boxWidth + gap);
            boxes[currentBox].posY = topOffset + row * (boxHeight + gap);
            currentBox++;
        }
    }
}
