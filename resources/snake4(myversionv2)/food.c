#include <ncurses.h>
#include <stdlib.h>
#include "food.h"
#include "snake.h"

void generate_food(Food *food, Snake *snake, int board_width, int board_height)
{
    bool valid_position;
    int x, y;

    do
    {
        valid_position = true;

        // Generate random x,y coordinates within game board
        x = rand() % board_width;
        y = (rand() % (board_height - SCORE_BOARD_HEIGHT)) + SCORE_BOARD_HEIGHT;

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

void draw_food(Food *food)
{
    if (food->active == true)
    {
        mvprintw(food->position.y + 1, food->position.x + 1, "*");
    }
}

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
