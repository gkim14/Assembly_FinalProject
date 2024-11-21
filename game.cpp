#include <iostream>
#include <ncurses.h>  // For terminal input and screen control
#include <unistd.h>   // For usleep()
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()

using std::time;
using std::rand;
using std::srand;


const int WIDTH = 80;   // Width of the field
const int HEIGHT = 25;  // Height of the field

const int PLAYER_PAIR = 1;
const int SPECIAL_PAIR = 2;
const int WALL_COUNT = 750;

const char WALL_CHAR = 'H';
const char PLAYER_CHAR = '@';
const char SPECIAL_CHAR = '#';
const char EMPTY_CHAR = ' ';
const char BORDER_CHAR = '*';


void drawField(char field[HEIGHT][WIDTH], int playerX, int playerY) {
    clear();  // Clear the screen

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY)
            {
                attron(COLOR_PAIR(PLAYER_PAIR));    // Player color
                mvaddch(y, x, PLAYER_CHAR);         // Draw the player character
                attroff(COLOR_PAIR(PLAYER_PAIR));   // Turn color off
            }
            else if(field[y][x] == SPECIAL_CHAR)
            {
                attron(COLOR_PAIR(SPECIAL_PAIR));   // Special color
                mvaddch(y, x, SPECIAL_CHAR);        // Draw the special character
                attroff(COLOR_PAIR(SPECIAL_PAIR));
            }
            else
                mvaddch(y, x, field[y][x]);         // Draw the field
        }
    }
    refresh();  // Refresh the screen to reflect changes
}

void drawBorder() {
    clear();  // Clear the screen

    // Draw top and bottom borders
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, BORDER_CHAR);              // Top border
        mvaddch(HEIGHT - 1, x, BORDER_CHAR);     // Bottom border
    }

    // Draw left and right borders
    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y, 0, BORDER_CHAR);              // Left border
        mvaddch(y, WIDTH - 1, BORDER_CHAR);      // Right border
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

    field[specialY][specialX] = SPECIAL_CHAR;  // Place the special tile
}

void placeWallTile(char field[HEIGHT][WIDTH], int& wallX, int& wallY, int playerX, int playerY) {
    do {
        wallX = rand() % WIDTH;  // Random column
        wallY = rand() % HEIGHT;  // Random row
    } while (wallX == playerX && wallY == playerY);  // Avoid placing on the player's position
    field[wallY][wallX] = WALL_CHAR;  // Place the special tile
}

int main() {
    srand(time(0));  // Seed the random number generator

    initscr();             // Initialize ncurses
    curs_set(0);           // Hide the cursor
    keypad(stdscr, true);  // Enable arrow keys
    noecho();              // Disable key echoing

    if(has_colors() == FALSE) // Check for colors
	{	
        endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();         //Start color 		
    init_pair(PLAYER_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(SPECIAL_PAIR, COLOR_RED, COLOR_MAGENTA);

    char field[HEIGHT][WIDTH];

    // Initialize the field with dots
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = EMPTY_CHAR;  // Fill the field with dots
        }
    }

    int playerX = WIDTH / 2;  // Start in the center
    int playerY = HEIGHT / 2;

    int wallX, wallY;
    //places walls randomly
    for(int i = 1; i< WALL_COUNT; ++i)
    placeWallTile(field, wallX, wallY, playerX, playerY);

    // Place the special tile randomly
    int specialX, specialY;
    placeSpecialTile(field, specialX, specialY, playerX, playerY);

    while (true) {
        drawField(field, playerX, playerY);

        int key = getch();  // Get the pressed key

        switch (key) {
            case KEY_UP:  // Arrow Up
                if (playerY > 0 && field[playerY-1][playerX] != WALL_CHAR) playerY--;
                break;
            case KEY_DOWN:  // Arrow Down
                if (playerY < HEIGHT - 1 && field[playerY+1][playerX] != WALL_CHAR) playerY++;
                break;
            case KEY_LEFT:  // Arrow Left
                if (playerX > 0 && field[playerY][playerX-1] != WALL_CHAR) playerX--;
                break;
            case KEY_RIGHT:  // Arrow Right
                if (playerX < WIDTH - 1 && field[playerY][playerX+1] != WALL_CHAR) playerX++;
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
                    if (field[y][x] == SPECIAL_CHAR) field[y][x] = EMPTY_CHAR;
                }
            }
            placeSpecialTile(field, specialX, specialY, playerX, playerY);
        }

        usleep(50000);  // Delay for smoother movement (50ms)
    }

    endwin();  // End ncurses mode
    return 0;
}
