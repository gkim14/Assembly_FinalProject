/**
 * Assembly Project Proof of Concept
 * Grace Kim and Jonathan Brough 
 * Nov 21, 2024
 * 
 * Proof of Concept for our game 
 * Compile and run using:
 *   g++ -o game game.cpp -lncurses
 *  ./game
 * 
 * Press esc twice to quit
 * Use WASD to move
 * Move to special char to start battle window
 * (Might get softlocked by walls)
 *
 * */ 

#include <iostream>
#include <ncurses.h>  // For terminal input and screen control
#include <unistd.h>   // For usleep()
#include <cstdlib>    // For rand() and srand()
#include <ctime>      // For time()

using std::time;
using std::rand;
using std::srand;
using std::string;


const int WIDTH = 80;           // Width of the field
const int HEIGHT = 25;          // Height of the field

const int PLAYER_PAIR = 1;      // for player color
const int SPECIAL_PAIR = 2;     // for special color
const int WALL_PAIR = 3;        // for wall color
const int WALL_COUNT = 750;     // Number of walls generated

const char WALL_CHAR = ':';     // Char for wall tile
const char PLAYER_CHAR = '@';   // Char for player tile
const char SPECIAL_CHAR = '#';  // Char for special tile
const char EMPTY_CHAR = ' ';    // Char for empty tiles
const char BORDER_CHAR = '*';   // Char for battle menu border

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
                attroff(COLOR_PAIR(SPECIAL_PAIR));  // Turn color off
            }
            else if(field[y][x] == WALL_CHAR)
            {
                attron(COLOR_PAIR(WALL_PAIR));   // Wall color
                mvaddch(y, x, WALL_CHAR);        // Draw the wall character
                attroff(COLOR_PAIR(WALL_PAIR));  // Turn color off
            }
            else
                mvaddch(y, x, field[y][x]);         // Draw the field
        }
    }
    refresh();  // Refresh the screen to reflect changes
}

// Basic placeholder for battle menu
void drawBorder() {
    clear();  // Clear the screen

    // Draw top and bottom borders
    for (int x = 0; x < WIDTH; x++) {
        mvaddch(0, x, BORDER_CHAR);              // Top border
        mvaddch(HEIGHT - 1, x, BORDER_CHAR);     // Bottom border
        if(x == (WIDTH/4))
            for(int y = 0; y < (int)(HEIGHT/4); y++) 
                mvaddch(HEIGHT-1-y, x, BORDER_CHAR); // Player menu side border
    }

    // Draw left and right borders
    for (int y = 0; y < HEIGHT; y++) {
        mvaddch(y, 0, BORDER_CHAR);              // Left border
        mvaddch(y, WIDTH - 1, BORDER_CHAR);      // Right border
        if(y == (int)(HEIGHT/4))
            for(int x = 0; x < WIDTH; x++)
            {
                mvaddch(HEIGHT - y, x, BORDER_CHAR);    // Player menu top
                mvaddch(y-1, WIDTH-1-x, BORDER_CHAR);   // other menu bottom
            }
    }

    refresh();  // Refresh the screen
    mvprintw(HEIGHT / 2 - 1, WIDTH / 2 - 8, "(Pretend enemy here)");
    mvprintw(HEIGHT / 2, WIDTH / 2 - 10, "Press any key to return");
    mvprintw(HEIGHT / 8, WIDTH / 2 - 8, "Cool menu stuff here");
    mvprintw(HEIGHT -(HEIGHT / 8), 5, "Player stats");
    mvprintw(HEIGHT -(HEIGHT / 8), (WIDTH-(WIDTH/4))/2 + WIDTH/4 - 10, "Player menu stuff");

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
    field[wallY][wallX] = WALL_CHAR;  // Place the wall tile
}

void intro(char field[HEIGHT][WIDTH])
{
    int counter = 0;
    string start_screen = 
"           _____            _          ____                         _           "
"          |_   _|___ __  __| |_       | __ )   __ _  ___   ___   __| |          "
"            | | / _ \\\\ \\/ /| __|_____ |  _ \\  / _` |/ __| / _ \\ / _` |          "
"            | ||  __/ >  < | |_|_____|| |_) || (_| |\\__ \\|  __/| (_| |          "
"            |_| \\___|/_/\\_\\ \\__|      |____/  \\__,_||___/ \\___| \\__,_|          "
"                               ____   ____    ____                              "
"                              |  _ \\ |  _ \\  / ___|                             "
"                              | |_) || |_) || |  _                              "
"                              |  _ < |  __/ | |_| |                             "
"                              |_| \\_\\|_|     \\____|                             "
;
    
    for(int y = 0; y < HEIGHT; y++)
        for(int x = 0; x< WIDTH; x++)
        {
            if(y > 2 && y < 13)
            {
                mvaddch(y, x, (char)(start_screen[counter]));
                counter++;
            }
            else
            {
                mvaddch(y, x, EMPTY_CHAR);
            }
        }
    mvprintw(HEIGHT - (HEIGHT/4), WIDTH/2 -12, "Press any button to start!");
    getch();        // Wait for any key to resume

    //basic info window
    drawField(field, -1, -1);
    mvprintw(HEIGHT/2-3, WIDTH/2 -22, "Welcome to this (sorta) text-based game!");
    mvprintw(HEIGHT/2-2, WIDTH/2 -10, "Use WASD to move.");
    mvprintw(HEIGHT/2-1, WIDTH/2 -21, "Interact with the # to start a battle.");
    mvprintw(HEIGHT/2+2, WIDTH/2 -17, "Press any button to continue.");
    getch();        // Wait for any key to resume

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
    init_pair(PLAYER_PAIR, COLOR_CYAN, COLOR_BLACK);        // Set player color
    init_pair(SPECIAL_PAIR, COLOR_MAGENTA, COLOR_YELLOW);   // Set special color
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_WHITE);         // Set wall color

    char field[HEIGHT][WIDTH];

    // Initialize the field with dots
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            field[y][x] = EMPTY_CHAR;  // Fill the field with dots
        }
    }

    //start screen here
    intro(field);

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
