#include "calico/gba/keypad.h"
#include "defs.h"
#include "logic.h"
#include "stdbool.h"
#include "view.h"
#include <nds.h>
#include <stdlib.h>
#include <time.h>

GameState game_state = IN_MENU;
MenuSelection menu_selection = START_GAME;

int main() {
  srand(time(NULL));
  init_consoles();

  while (pmMainLoop()) {
    swiWaitForVBlank();
    scanKeys();

    u32 keys_down = keysDown();

    if (game_state == IN_MENU) {
      print_title();
      print_menu(menu_selection);

      if (keys_down & KEY_A) {
        if (menu_selection == START_GAME) {

          start_game();

          game_state = IN_GAME;
        }
        else if (menu_selection == QUIT) {
          abort();
        }
      }
      else if (keys_down & KEY_UP) {
        if (menu_selection == QUIT) {
          menu_selection = START_GAME;
        }
      }
      else if (keys_down & KEY_DOWN) {
        if (menu_selection == START_GAME) {
          menu_selection = QUIT;
        }
      }
    }
    else if (game_state == IN_GAME) {
      print_scoreboard();
      print_grid(false, false);

      bool game_over = false;

      if (keys_down & KEY_LEFT) {
        game_over = shift_grid(DIR_LEFT);
      }
      else if (keys_down & KEY_RIGHT) {
        game_over = shift_grid(DIR_RIGHT);
      }
      else if (keys_down & KEY_UP) {
        game_over = shift_grid(DIR_UP);
      }
      else if (keys_down & KEY_DOWN) {
        game_over = shift_grid(DIR_DOWN);
      }
      else if (keys_down & KEY_START) {
        game_state = PAUSED;
      }

      if (game_over) {
        game_state = GAME_OVER;
      }
    }
    else if (game_state == PAUSED) {
      print_scoreboard();
      print_grid(false, true);

      if (keys_down & KEY_START) {
        game_state = IN_MENU;
      }
      else if (keys_down & KEY_LEFT) {
        game_state = IN_GAME;
      }
      else if (keys_down & KEY_RIGHT) {
        game_state = IN_GAME;
      }
      else if (keys_down & KEY_UP) {
        game_state = IN_GAME;
      }
      else if (keys_down & KEY_DOWN) {
        game_state = IN_GAME;
      }
    }
    else if (game_state == GAME_OVER) {
      print_scoreboard();
      print_grid(true, false);

      if (keys_down & KEY_START) {
        game_state = IN_MENU;
      }
    }
  }

  return 0;
}
