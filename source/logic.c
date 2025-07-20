#include "logic.h"
#include "defs.h"
#include "stdbool.h"
#include "stdlib.h"
#include <nds.h>

int grid[HEIGHT][WIDTH];
int score;

void insert_randomly() {
  int value = (rand() % 10 == 0) ? 4 : 2;
  int x, y;

  do {
    x = rand() % WIDTH;
    y = rand() % HEIGHT;
  } while (grid[y][x] != 0);

  grid[y][x] = value;
}

void initialize_score() {
  score = 0;
}

void initialize_grid() {
  int x, y;

  for (y = 0; y < HEIGHT; y++) {
    for (x = 0; x < WIDTH; x++) {
      grid[y][x] = 0;
    }
  }

  insert_randomly();
  insert_randomly();
}

void start_game() {
  initialize_grid();
  initialize_score();
}

MovementResult try_move_or_merge_tile(int *current, int *next, bool can_merge) {
  if (*next == 0) {
    *next = *current;
    *current = 0;

    return MOVE_MOVED;
  }
  else if (*next == *current && can_merge) {
    *next = *current * 2;
    *current = 0;

    score += *next;

    return MOVE_MERGED;
  }
  else {
    return MOVE_STUCK;
  }
}

void slide_tile(int x, int y, Direction direction, bool *anything_moved, bool can_merge) {
  if (direction == DIR_LEFT && x > 0) {
    MovementResult result = try_move_or_merge_tile(&grid[y][x], &grid[y][x - 1], can_merge);

    if (result != MOVE_STUCK) {
      *anything_moved = true;

      slide_tile(x - 1, y, DIR_LEFT, anything_moved, !(result == MOVE_MERGED));
    }
  }

  if (direction == DIR_RIGHT && x < WIDTH - 1) {
    MovementResult result = try_move_or_merge_tile(&grid[y][x], &grid[y][x + 1], can_merge);

    if (result != MOVE_STUCK) {
      *anything_moved = true;
      slide_tile(x + 1, y, DIR_RIGHT, anything_moved, !(result == MOVE_MERGED));
    }
  }

  if (direction == DIR_UP && y > 0) {
    MovementResult result = try_move_or_merge_tile(&grid[y][x], &grid[y - 1][x], can_merge);

    if (result != MOVE_STUCK) {
      *anything_moved = true;
      slide_tile(x, y - 1, DIR_UP, anything_moved, !(result == MOVE_MERGED));
    }
  }

  if (direction == DIR_DOWN && y < HEIGHT - 1) {
    MovementResult result = try_move_or_merge_tile(&grid[y][x], &grid[y + 1][x], can_merge);

    if (result != MOVE_STUCK) {
      *anything_moved = true;
      slide_tile(x, y + 1, DIR_DOWN, anything_moved, !(result == MOVE_MERGED));
    }
  }
}

void shift_row(int y, Direction direction, bool *anything_moved) {
  int start_x, limit_x, delta_x;

  if (direction == DIR_LEFT) {
    start_x = 0;
    delta_x = 1;
    limit_x = WIDTH;
  }
  else if (direction == DIR_RIGHT) {
    start_x = WIDTH - 1;
    delta_x = -1;
    limit_x = -1;
  }
  else {
    abort(); // should never happen
  }

  for (int x = start_x; x != limit_x; x += delta_x) {
    slide_tile(x, y, direction, anything_moved, true);
  }
}

void shift_col(int x, Direction direction, bool *anything_moved) {
  int start_y, limit_y, delta_y;

  if (direction == DIR_UP) {
    start_y = 0;
    delta_y = 1;
    limit_y = HEIGHT;
  }
  else if (direction == DIR_DOWN) {
    start_y = HEIGHT - 1;
    delta_y = -1;
    limit_y = -1;
  }
  else {
    abort(); // should never happen
  }

  for (int y = start_y; y != limit_y; y += delta_y) {
    slide_tile(x, y, direction, anything_moved, true);
  }
}

void shift_all_rows(bool *anything_moved, Direction direction) {
  if (direction != DIR_LEFT && direction != DIR_RIGHT) {
    abort(); // should never happen
  }

  for (int y = 0; y < HEIGHT; y++) {
    shift_row(y, direction, anything_moved);
  }
}

void shift_all_cols(bool *anything_moved, Direction direction) {
  if (direction != DIR_UP && direction != DIR_DOWN) {
    abort(); // should never happen
  }

  for (int x = 0; x < WIDTH; x++) {
    shift_col(x, direction, anything_moved);
  }
}

bool is_number_stuck(int x, int y) {
  int value = grid[y][x];

  bool can_merge_up = y != 0 && value == grid[y - 1][x];
  bool can_merge_down = y != HEIGHT - 1 && value == grid[y + 1][x];
  bool can_merge_left = x != 0 && value == grid[y][x - 1];
  bool can_merge_right = x != WIDTH - 1 && value == grid[y][x + 1];

  bool can_merge = can_merge_up || can_merge_down || can_merge_left || can_merge_right;

  bool can_move_up = y != 0 && grid[y - 1][x] == 0;
  bool can_move_down = y != HEIGHT - 1 && grid[y + 1][x] == 0;
  bool can_move_left = x != 0 && grid[y][x - 1] == 0;
  bool can_move_right = x != WIDTH - 1 && grid[y][x + 1] == 0;

  bool can_move = can_move_up || can_move_down || can_move_left || can_move_right;

  return !can_merge && !can_move;
}

bool is_grid_stuck() {
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      if (grid[y][x] != 0 && !is_number_stuck(x, y)) {
        return false;
      }
    }
  }

  return true;
}

bool shift_grid(Direction direction) {
  bool anything_moved = false;

  if (direction == DIR_LEFT || direction == DIR_RIGHT) {
    shift_all_rows(&anything_moved, direction);
  }
  else if (direction == DIR_UP || direction == DIR_DOWN) {
    shift_all_cols(&anything_moved, direction);
  }

  if (anything_moved == true) {
    insert_randomly();
  }

  return is_grid_stuck();
}
