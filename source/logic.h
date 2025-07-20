#ifndef NDS2048_LOGIC_H
#define NDS2048_LOGIC_H

#include "defs.h"
#include "stdbool.h"

extern int grid[HEIGHT][WIDTH];
extern int score;

void start_game();

bool shift_grid(Direction direction);

#endif
