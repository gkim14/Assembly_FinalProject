#include <iostream>
#include <ncurses.h>  // For terminal input and screen control
#include <unistd.h>   // For usleep()
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()

using namespace std;

const int WIDTH = 80;   // Width of the field
const int HEIGHT = 25;  // Height of the field

void drawField(char field[HEIGHT][WIDTH], int playerX, int playerY) {
    clear();  // Clear the screen

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY)
                mvaddch(y, x, '@');  // Draw the player character
            else
                mvaddch(y, x, field[y][x]);  // Draw the field
        }
    }
    refresh();  // Refresh the screen to reflect changes
}

void drawBorder() {
    clear();  // Clear the screen

    // Draw top and bottom borders
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, '*');              // Top border
        mvaddch(HEIGHT - 1, x, '*');     // Bottom border
    }

    // Draw left and right borders
    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y, 0, '*');              // Left border
        mvaddch(y, WIDTH - 1, '*');      // Right border
    }

    refresh();  // Refresh the screen
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Press any key to return");
    refresh();
}

void placeSpecialTile(char field[HEIGHT][WIDTH], int& specialX, int& specialY, int playerX, int playerY) {
    do {
        specialX = rand() % WIDTH;  // Random column
        specialY = rand() % HEIGHT;  // Random row
    } while (specialX == playerX && specialY == playerY);  // Avoid placing on the player's position

    field[specialY][specialX] = '#';  // Place the special tile
}

int main() {
    srand(time(0));  // Seed the random number generator

    initscr();             // Initialize ncurses
    curs_set(0);           // Hide the cursor
    keypad(stdscr, true);  // Enable arrow keys
    noecho();              // Disable key echoing

    char field[HEIGHT][WIDTH];

    // Initialize the field with dots
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = '.';  // Fill the field with dots
        }
    }

    int playerX = WIDTH / 2;  // Start in the center
    int playerY = HEIGHT / 2;

    // Place the special tile randomly
    int specialX, specialY;
    placeSpecialTile(field, specialX, specialY, playerX, playerY);

    while (true) {
        drawField(field, playerX, playerY);

        int key = getch();  // Get the pressed key

        switch (key) {
            case KEY_UP:  // Arrow Up
                if (playerY > 0) playerY--;
                break;
            case KEY_DOWN:  // Arrow Down
                if (playerY < HEIGHT - 1) playerY++;
                break;
            case KEY_LEFT:  // Arrow Left
                if (playerX > 0) playerX--;
                break;
            case KEY_RIGHT:  // Arrow Right
                if (playerX < WIDTH - 1) playerX++;
                break;
            case 27:  // Escape key to exit
                endwin();  // End ncurses mode
                return 0;
        }

        // Check if the player moved onto the special tile
        if (playerX == specialX && playerY == specialY) {
            drawBorder();   // Show border screen
            getch();        // Wait for any key to resume

            // Reset player position and place a new special tile
            playerX = WIDTH / 2;
            playerY = HEIGHT / 2;
            for (int y = 0; y < HEIGHT; y++) {  // Clear the previous special tile
                for (int x = 0; x < WIDTH; x++) {
                    if (field[y][x] == '#') field[y][x] = '.';
                }
            }
            placeSpecialTile(field, specialX, specialY, playerX, playerY);
        }

        usleep(50000);  // Delay for smoother movement (50ms)
    }

    endwin();  // End ncurses mode
    return 0;
}
