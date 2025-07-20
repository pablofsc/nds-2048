#ifndef NDS2048_DEFS_H
#define NDS2048_DEFS_H

#define HEIGHT 4
#define WIDTH 4

#define ANSI_RESET "\x1b[0m"
#define ANSI_BLACK "\x1b[30m"
#define ANSI_DARK_RED "\x1b[31m"
#define ANSI_DARK_GREEN "\x1b[32m"
#define ANSI_DARK_YELLOW "\x1b[33m"
#define ANSI_DARK_BLUE "\x1b[34m"
#define ANSI_DARK_MAGENTA "\x1b[35m"
#define ANSI_DARK_CYAN "\x1b[36m"
#define ANSI_GRAY "\x1b[37m"
#define ANSI_DARK_GRAY "\x1b[38m"
#define ANSI_RED "\x1b[41m"
#define ANSI_GREEN "\x1b[42m"
#define ANSI_YELLOW "\x1b[43m"
#define ANSI_BLUE "\x1b[44m"
#define ANSI_MAGENTA "\x1b[45m"
#define ANSI_CYAN "\x1b[46m"

typedef enum {
  IN_MENU,
  IN_GAME,
  PAUSED,
  GAME_OVER
} GameState;

typedef enum {
  CONTINUE_GAME,
  START_GAME,
  QUIT
} MenuSelection;

typedef enum {
  DIR_RIGHT,
  DIR_LEFT,
  DIR_DOWN,
  DIR_UP
} Direction;

typedef enum {
  MOVE_STUCK,
  MOVE_MOVED,
  MOVE_MERGED
} MovementResult;

#endif
