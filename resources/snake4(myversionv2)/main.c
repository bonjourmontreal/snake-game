// Snake Game
// Use arrow keys to control the snake
// Eat food (*) to increase the snake's length
// Avoid hitting the snake's body or the walls
// Press 'q' or 'Q' to quit the game
//
// Compile: gcc -o snakegame snakegame.c -lncurses
// Run: ./snakegame

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include "point.h"
#include "snake.h"
#include "food.h"

void init_ncurses();
void draw_board();

int main()
{
    srand(time(NULL));
    Snake snake;
    Food food;

    // Initialize terminal for gameplay
    init_ncurses();
    draw_board();

    generate_snake(&snake, BOARD_WIDTH, BOARD_HEIGHT);
    generate_food(&food, &snake, BOARD_WIDTH, BOARD_HEIGHT);

    game_loop(&snake, &food, BOARD_WIDTH, BOARD_HEIGHT);

    // Clean up and close ncurses
    endwin(); 

    printf("\n--- GAME OVER ---\n");
    printf("Your score: %i\n\n", snake.lenght - INITIAL_SNAKE_LENGTH);
    
    return 0;
}

void init_ncurses()
{
    initscr(); // Initialize the library
    cbreak(); // Disable line buffering, but allow interrupt keys
    keypad(stdscr, TRUE); // Enable special keys
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor
    timeout(100); // Set getch() non-blocking with 100ms delay
}

void draw_board()
{
    for (int i = 1; i < BOARD_HEIGHT + 3; i++)
    {
        for (int j = 0; j < BOARD_WIDTH + 2; j++)
        {
            if (i == 1 || i == BOARD_HEIGHT + 2 || j == 0 || j == BOARD_WIDTH + 1)
            {
                mvprintw(i, j, "#"); 
            }
        }
    }
}
