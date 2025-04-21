# "No Deal" for N64

A "Deal or No Deal" style game for the Nintendo 64, built using libdragon with the **preview** branch.

## Screenshots
![No Deal Main Menu](https://github.com/theokyr/no-deal-n64/raw/master/docs/img/no-deal-1.png "No Deal Main Menu")
![No Deal Gameplay](https://github.com/theokyr/no-deal-n64/raw/master/docs/img/no-deal-2.png "No Deal Gameplay")

## Features

*   Classic Deal or No Deal gameplay.
*   Select your box and open the others round by round.
*   Receive offers from the Banker based on the remaining values.
*   Decide: Deal or No Deal?

## Installation

1.  **Install LibDragon Toolchain:**
    Follow [the official Libdragon CLI installation guide for your operating system](https://github.com/DragonMinded/libdragon#getting-started-how-to-build-a-rom).

2.  Clone the Repository
3.  **Navigate into the Directory:**
    ```bash
    cd no-deal-v2
    ```

4.  **Initialize LibDragon Project:**
    ```bash
    libdragon init
    ```

5.  **Build:**
    (outputs a region-free ROM `no_deal.z64`).
    ```bash
    libdragon make
    ```

## Running the Game

After building, you will have a ROM file (e.g., `no_deal.z64`) in the project's root directory.

*   **Emulator:** Load the ROM file into your preferred N64 emulator (e.g., Ares, Mupen64Plus, Simple64).
*   **Flashcart:** Copy the ROM file to the SD card of your N64 flashcart (e.g., EverDrive64, 64drive) and load it on your console.

Enjoy the game! 