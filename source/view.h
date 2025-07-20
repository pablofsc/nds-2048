#ifndef NDS2048_VIEW_H
#define NDS2048_VIEW_H

#include "defs.h"
#include <nds.h>

void init_consoles();

void print_scoreboard();

void print_menu(MenuSelection selection);

void print_grid(bool is_game_over, bool is_paused);

#endif
