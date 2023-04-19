#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_WIDTH 30
#define BOARD_HEIGHT 20
#define MAX_SNAKE_LENGTH (BOARD_WIDTH * BOARD_HEIGHT)

// enum data type representing the possible directions of the snake
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Define a structure to hold a point (x, y coordinates)
typedef struct {
    int x;
    int y;
} Point;

// The Snake structure, consisting of an array of segments, its length, and its current direction
typedef struct {
    Point segments[MAX_SNAKE_LENGTH];
    int length;
    Direction direction; // Added direction member to the Snake structure
} Snake;

// Define a structure to hold the food data
typedef struct {
    Point position; // The x, y coordinates of the food
    bool active;    // Whether the food is active on the game board
} Food;

void init_ncurses();

void draw_board();

void init_snake(Snake *snake);
void update_snake(Snake *snake, int ch);
void draw_snake(Snake *snake) ;

void generate_food(Food *food, Snake *snake);
void draw_food(Food *food) ;

bool check_snake_food_collision(Snake *snake, Food *food);
bool check_wall_collision(Snake *snake);
bool check_self_collision(Snake *snake);

// gcc -o snakegame snakegame.c -lncurses
int main()
{
    // Initialize ncurses
    init_ncurses();

    // Create a Snake instance
    Snake snake;

    // Initialize the snake
    init_snake(&snake);

    // Seed the random number generator
    srand(time(NULL));

    // Create a Food instance
    Food food;

    // Generate the initial food
    generate_food(&food, &snake);

    int ch;

    // Game loop
    while (1)
    {
        // Draw the game board
        draw_board();

        // Draw the snake
        draw_snake(&snake);

        // Draw the food
        draw_food(&food);

        // Get user input and update game state
        ch = getch();

        // If the user presses 'q', exit the game loop
        if (ch == 'q' || ch == 'Q')
        {
            break;
        }

        // Update the snake's position based on user input
        update_snake(&snake, ch);

        // Refresh the screen
        refresh();

        // Check if the snake has collided with the food
        if (check_snake_food_collision(&snake, &food)) 
        {
            // Increase the snake's length
            snake.length++;

            // Generate a new food item
            generate_food(&food, &snake);
        }

        // Check for game over conditions
        if (check_wall_collision(&snake) || check_self_collision(&snake)) 
        {
            // End the game
            break;
        }
    }

    // Clean up and close ncurses
    endwin();

    return 0;
}

void init_ncurses()
{
    initscr(); // Initialize the library
    raw(); // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys
    noecho(); // Don't echo user input
    curs_set(0); // Hide the cursor
    timeout(100); // Set getch() non-blocking with 100ms delay
}

void draw_board()
{
    for (int i = 0; i < BOARD_HEIGHT + 2; i++)
    {
        for (int j = 0; j < BOARD_WIDTH + 2; j++)
        {
            if (i == 0 || i == BOARD_HEIGHT + 1 || j == 0 || j == BOARD_WIDTH + 1)
            {
                mvprintw(i, j, "#");
            }
            else
            {
                mvprintw(i, j, " ");
            }
        }
    }
}

void init_snake(Snake *snake)
{
    snake->length = 1;
    snake->segments[0].x = BOARD_WIDTH / 2;
    snake->segments[0].y = BOARD_HEIGHT / 2;
    snake->direction = RIGHT; // Set the initial direction to RIGHT
}

void update_snake(Snake *snake, int ch)
{
    // Update the snake's direction based on user input
    if (ch == KEY_UP && snake->direction != DOWN)
    {
        snake->direction = UP;
    }
    else if (ch == KEY_DOWN && snake->direction != UP)
    {
        snake->direction = DOWN;
    }
    else if (ch == KEY_LEFT && snake->direction != RIGHT)
    {
        snake->direction = LEFT;
    }
    else if (ch == KEY_RIGHT && snake->direction != LEFT)
    {
        snake->direction = RIGHT;
    }

    // Move the snake's segments
    for (int i = snake->length - 1; i > 0; i--)
    {
        // Erase the previous position of the snake's tail segment
        mvprintw(snake->segments[i].y + 1, snake->segments[i].x + 1, " ");

        snake->segments[i] = snake->segments[i - 1];
    }

    // Update the head of the snake based on its current direction
    switch (snake->direction)
    {
        case UP:
            snake->segments[0].y -= 1;
            break;
        case DOWN:
            snake->segments[0].y += 1;
            break;
        case LEFT:
            snake->segments[0].x -= 1;
            break;
        case RIGHT:
            snake->segments[0].x += 1;
            break;
    }
}

void draw_snake(Snake *snake)
{
    for (int i = 0; i < snake->length; i++) 
    {
        // Clear the previous position of the segment
        mvprintw(snake->segments[i].y + 1, snake->segments[i].x + 1, " ");

        // Draw the segment in the new position
        mvprintw(snake->segments[i].y + 1, snake->segments[i].x + 1, i == 0 ? "@" : "o");
    }
}

void generate_food(Food *food, Snake *snake) 
{
    bool valid_position;
    int x, y;

    do 
    {
        valid_position = true;

        // Generate random x and y coordinates within the game board
        x = rand() % BOARD_WIDTH;
        y = rand() % BOARD_HEIGHT;

        // Check if the generated position is inside the snake's body
        for (int i = 0; i < snake->length; i++) 
        {
            if (snake->segments[i].x == x && snake->segments[i].y == y) 
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

void draw_food(Food *food) 
{
    if (food->active) 
    {
        mvprintw(food->position.y + 1, food->position.x + 1, "*");
    }
}

bool check_snake_food_collision(Snake *snake, Food *food) 
{
    if (snake->segments[0].x == food->position.x && snake->segments[0].y == food->position.y) 
    {
        food->active = false; // Deactivate the food item
        return true;
    }
    else 
    {
        return false;
    }
}

bool check_wall_collision(Snake *snake)
{
    int head_x = snake->segments[0].x;
    int head_y = snake->segments[0].y;

    return head_x < 0 || head_x >= BOARD_WIDTH || 
           head_y < 0 || head_y >= BOARD_HEIGHT;
}

bool check_self_collision(Snake *snake) 
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