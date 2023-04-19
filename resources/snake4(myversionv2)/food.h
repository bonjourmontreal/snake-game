#ifndef FOOD_H
#define FOOD_H

#include <stdbool.h>
#include "point.h"

struct Snake;  // Forward declare the Snake struct

typedef struct
{
    Point position;
    bool active;
} Food;

void generate_food(Food *food, struct Snake *snake, int board_width, int board_height);
void draw_food(Food *food);
bool check_snake_food_collision(struct Snake *snake, Food *food);

#endif // FOOD_H