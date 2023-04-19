// Snake Game
// Use arrow keys to control the snake
// Eat food (*) to increase the snake's length
// Avoid hitting the snake's body or the walls
// Press 'q' or 'Q' to quit the game
//
// Compile: gcc -o snakegame snakegame.c -lncurses
// Run: ./snakegame

#include <ncurses.h>
#include <time.h> // time()
#include <stdlib.h> // srand()
#include <stdbool.h> 
#include <unistd.h> // for sleep()

// Constants
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 50 
#define MAX_SNAKE_LENGTH (BOARD_HEIGHT * BOARD_WIDTH)
#define INITIAL_SNAKE_LENGTH 3
#define INITIAL_GAME_SPEED 100
#define SCORE_BOARD_HEIGHT 1

// Enumerations
typedef enum
{
    UP, // First element usually associated with 0
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Structures
typedef struct 
{
    int x;
    int y;
}Point;

typedef struct
{
    int length;
    Point segments[MAX_SNAKE_LENGTH];
    Direction direction;
}Snake;

typedef struct
{
    bool active;
    Point position;
}Food;

// Function prototypes
void init_ncurses(); // Initializes ncurses and sets up terminal for gameplay
void draw_board(); // Draws the game board
void generate_snake(Snake *snake); // Generates the initial snake
void draw_snake(Snake *snake); // Draws the snake on the game board
void handle_input(Snake *snake, int ch); // Update direction based on input
void move_snake(Snake *snake, int ch); // Updates the position and direction of the snake based on user input
void generate_food(Food *food, Snake *snake); // Generates the food at a random location on the game board
void draw_food(Food *food); // Draws the food on the game board
bool check_snake_food_collision(Snake *snake, Food *food); // Checks if the snake has collided with the food
void grow_snake(Snake *snake); // Increments the lenght of snake when collision with food
bool check_snake_self_collision(Snake *snake); // Checks if the snake has collided with itself
bool check_snake_wall_collision(Snake *snake); // Checks if the snake has collided with the walls of the game board
void display_score(Snake *snake); // Displays the player's score on the game board
int game_speed(Snake *snake); // Calculates the game speed based on the length of the snake
void game_loop(Snake *snake, Food *food); // Main game loop
void game_over_screen(Snake *snake); // Display game over screen

// Main function
int main()
{
    int ch;
    srand(time(NULL));
    Snake snake;
    Food food;

    // Initialize terminal for gameplay
    init_ncurses();
    draw_board();

    generate_snake(&snake);
    generate_food(&food, &snake);

    game_loop(&snake, &food);
    
    // Show game over screen
    game_over_screen(&snake);

    // Clean up and close ncurses
    endwin(); 
    
    return 0;
}

// Initializes ncurses and sets up terminal for gameplay
void init_ncurses()
{
    initscr(); // Initialize the library
    cbreak(); // Disable line buffering, but allow interrupt keys
    keypad(stdscr, TRUE); // Enable special keys
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor
    timeout(100); // Set getch() non-blocking with 100ms delay
}

// Draws the game board
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

// Generates the initial snake
void generate_snake(Snake *snake)
{
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->direction = RIGHT;
    snake->segments[0].x = BOARD_WIDTH / 2;
    snake->segments[0].y = BOARD_HEIGHT / 2; 
}

// Update direction based on input
void handle_input(Snake *snake, int ch)
{
    if (ch == KEY_UP && snake->direction != DOWN)
    {
        snake->direction = UP;
    }
    else if (ch == KEY_DOWN && snake->direction != UP)
    {
        snake->direction = DOWN;
    }
    else if (ch == KEY_RIGHT && snake->direction != LEFT)
    {
        snake->direction = RIGHT;
    }
    else if (ch == KEY_LEFT && snake->direction != RIGHT)
    {
        snake->direction = LEFT;
    }
}

// Updates the position and direction of the snake based on user input
void move_snake(Snake *snake, int ch)
{
    // Clear previous position of tail
    mvprintw(snake->segments[snake->length - 1].y + 1, snake->segments[snake->length - 1].x + 1, " ");

    // Move snake segments
    for (int i = snake->length - 1; i > 0; i--)
    {
        // Update snake segment location
        snake->segments[i] = snake->segments[i - 1];
    }
    
    // Update head of snake based on direction
    switch (snake->direction)
    {
        case UP:
            snake->segments[0].y -= 1;
            break;

        case DOWN:
            snake->segments[0].y += 1;
            break;
        case RIGHT:
            snake->segments[0].x += 1;
            break;
        case LEFT:
            snake->segments[0].x -= 1;
            break;
    }
}

// Draws the snake on the game board
void draw_snake(Snake *snake)
{
    for (int i = 0; i < snake->length; i++)
    {
        // Draw the segment in the new position
        mvprintw(snake->segments[i].y + 1, snake->segments[i].x + 1, i == 0 ? "@" : "o");
    }
}

// Generates the food at a random location on the game board
void generate_food(Food *food, Snake *snake)
{
    bool valid_position;
    int x, y;

    do
    {
        valid_position = true;

        // Generate random x,y coordinates within game board
        x = rand() % BOARD_WIDTH;
        y = (rand() % (BOARD_HEIGHT)) + SCORE_BOARD_HEIGHT;

        // Check if generate coordinates inside snake's body
        for (int i = 0; i < snake->length; i++) 
        {
            if (x == snake->segments[i].x && y == snake->segments[i].y)
            {
                valid_position = false;
                break;
            }
        }
    } while (!valid_position);
    
    food->position.x = x;
    food->position.y = y;
    food->active = true;
}

// Draws the food on the game board
void draw_food(Food *food)
{
    if (food->active == true)
    {
        mvprintw(food->position.y + 1, food->position.x + 1, "*");
    }
}

// Checks if the snake has collided with the food
bool check_snake_food_collision(Snake *snake, Food *food)
{
    if (snake->segments[0].x == food->position.x &&
        snake->segments[0].y == food->position.y)
    {
        food->active = false;
        return true;
    }
    else
    {
        return false;
    }
}

// Increments the lenght of snake when collision with food
void grow_snake(Snake *snake) 
{
    snake->length++;
}

// Checks if the snake has collided with itself
bool check_snake_self_collision(Snake *snake)
{
    for (int i = 1; i < snake->length; i++)
    {
        if (snake->segments[0].x == snake->segments[i].x &&
            snake->segments[0].y == snake->segments[i].y)
        {
            return true;
        }
    }

    return false;
}

// Checks if the snake has collided with the walls of the game board
bool check_snake_wall_collision(Snake *snake)
{
    if (snake->segments[0].x < 0 || snake->segments[0].x >= BOARD_WIDTH ||
        snake->segments[0].y < SCORE_BOARD_HEIGHT || snake->segments[0].y >= BOARD_HEIGHT + SCORE_BOARD_HEIGHT)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// Displays the player's score on the game board
void display_score(Snake *snake)
{
    // Position the score at the top center of the grid
    int score = snake->length - INITIAL_SNAKE_LENGTH;
    mvprintw(0, (BOARD_WIDTH / 2) - 4, "Score: %d", score);
}

// Calculates the game speed based on the length of the snake
int game_speed(Snake *snake)
{
    int speed = INITIAL_GAME_SPEED - (snake->length * 5);
    if (speed < 50)
    {
        speed = 50;
    }
    return speed;
}

// Main game loop
void game_loop(Snake *snake, Food *food)
{
    while(1)
    {
        draw_snake(snake);
        draw_food(food);

        int ch = getch();

        if(ch == 'q' || ch == 'Q')
        {
            break; 
        }  
        
        handle_input(snake, ch);
        move_snake(snake, ch);

        if (check_snake_food_collision(snake, food))
        {
            grow_snake(snake);
            generate_food(food, snake);
        }

        if (check_snake_self_collision(snake) || check_snake_wall_collision(snake))
        {
            break;
        }

        display_score(snake);
        refresh();

        // Wait based on current game speed before the next loop iteration
        timeout(game_speed(snake));
        
    }
}

// Display game over screen
void game_over_screen(Snake *snake)
{
    clear();
    mvprintw(BOARD_HEIGHT / 2, (BOARD_WIDTH / 2) - 5, "GAME OVER!");
    mvprintw((BOARD_HEIGHT / 2) + 1, (BOARD_WIDTH / 2) - 7, "Final Score: %d", snake->length - INITIAL_SNAKE_LENGTH);
    mvprintw((BOARD_HEIGHT / 2) + 2, (BOARD_WIDTH / 2) - 12, "Press any key to continue.");
    refresh();
    timeout(-1); // Set getch() to blocking mode
    sleep(1);
    getch();// Wait for user to press a key
}
