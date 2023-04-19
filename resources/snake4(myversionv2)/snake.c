#include <ncurses.h>
#include <stdlib.h>
#include "snake.h"
#include "food.h"

void generate_snake(Snake *snake, int board_width, int board_height)
{
    snake->lenght = INITIAL_SNAKE_LENGTH;
    snake->direction = RIGHT;
    snake->segments[0].x = BOARD_WIDTH / 2;
    snake->segments[0].y = BOARD_HEIGHT / 2;
}

void draw_snake(Snake *snake) 
{
    for (int i = 0; i < snake->lenght; i++)
    {
        // Draw the segment in the new position
        mvprintw(snake->segments[i].y + 1, snake->segments[i].x + 1, i == 0 ? "@" : "o");
    }
}

void update_snake(Snake *snake, int ch)
{
    // Clear previous position of tail
    mvprintw(snake->segments[snake->lenght - 1].y + 1, snake->segments[snake->lenght - 1].x + 1, " ");

    // Update direction based on input
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
    else if(ch == KEY_LEFT && snake->direction != RIGHT)
    {
        snake->direction = LEFT;
    }

    // Move snake segments
    for (int i = snake->lenght - 1; i > 0; i--)
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

bool check_snake_self_collision(Snake *snake)
{
    for (int i = 1; i < snake->lenght; i++)
    {
        if (snake->segments[0].x == snake->segments[i].x &&
            snake->segments[0].y == snake->segments[i].y)
        {
            return true;
        }
    }

    return false;
}

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



