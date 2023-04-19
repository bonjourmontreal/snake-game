#ifndef SNAKE_H
#define SNAKE_H

#include <stdbool.h>
#include "point.h"
#include "food.h"

#define BOARD_HEIGHT 20
#define BOARD_WIDTH 50 
#define MAX_SNAKE_LENGTH (BOARD_HEIGHT * BOARD_WIDTH)
#define INITIAL_SNAKE_LENGTH 3
#define INITIAL_GAME_SPEED 100
#define SCORE_BOARD_HEIGHT 1

typedef enum
{
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct
{
    int length;
    Point segments[MAX_SNAKE_LENGTH];
    Direction direction;
} Snake;

void generate_snake(Snake *snake, int board_width, int board_height);
void game_loop(Snake *snake, Food *food, int board_width, int board_height);

#endif // SNAKE_H