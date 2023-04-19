#ifndef FOOD_H
#define FOOD_H

typedef struct Snake Snake; // Add this line
typedef struct Food Food;   // Add this line

#include <stdbool.h>
#include "point.h"

typedef struct
{
    Point position;
    bool active;
} Food;

void generate_food(Food *food, Snake *snake, int board_width, int board_height);
bool check_snake_food_collision(Snake *snake, Food *food);
void draw_food(Food *food);

#endif // FOOD_H
