#include "defs.h"
#include "logic.h"
#include "nds/arm9/console.h"
#include <nds.h>
#include <stdio.h>

PrintConsole screen_top;
PrintConsole screen_btm;

char *center_number(int number, int length) {
  char num_str[32];
  snprintf(num_str, sizeof(num_str), "%d", number);
  int num_len = strlen(num_str);

  if (num_len >= length) {
    char *result = malloc(num_len + 1);
    strcpy(result, num_str);
    return result;
  }

  int total_padding = length - num_len;
  int left_padding = total_padding / 2;

  char *result = malloc(length + 1);
  memset(result, ' ', length);
  memcpy(result + left_padding, num_str, num_len);
  result[length] = '\0';

  return result;
}

void init_consoles() {
  videoSetMode(MODE_0_2D);
  videoSetModeSub(MODE_0_2D);

  vramSetBankA(VRAM_A_MAIN_BG);
  vramSetBankC(VRAM_C_SUB_BG);

  consoleInit(&screen_top, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
  consoleInit(&screen_btm, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
}

void print_scoreboard() {
  consoleSelect(&screen_top);
  consoleClear();

  char *centered_str = center_number(score, 9);

  iprintf("\n\n\n\n\n\n\n\n");
  iprintf("--------------------------------\n");
  iprintf("             SCORE              \n\n");
  iprintf("           %s\n\n", centered_str);
  iprintf("--------------------------------");

  free(centered_str);
}

void print_menu_option(bool is_selected, char text[]) {
  iprintf("\n");

  if (is_selected) {
    iprintf("%s > %s %s\n", ANSI_RESET, text, ANSI_GRAY);
  }
  else {
    iprintf("%s   %s %s\n", ANSI_DARK_GRAY, text, ANSI_GRAY);
  }
}

void print_menu(MenuSelection selection) {
  consoleSelect(&screen_top);
  consoleClear();

  consoleSelect(&screen_btm);
  consoleClear();

  print_menu_option(selection == START_GAME, "Start a new game");
  print_menu_option(selection == QUIT, "Quit");
}

void print_grid(bool is_game_over, bool is_paused) {
  consoleSelect(&screen_btm);
  consoleClear();

  int x, y;

  if (is_game_over) {
    iprintf("\n           GAME  OVER           \n");
  }
  else if (is_paused) {
    iprintf("\n             PAUSED             \n");
  }
  else {
    iprintf("\n\n\n");
  }

  for (y = 0; y < HEIGHT; y++) {
    iprintf("\n\n");

    iprintf("      ");

    for (x = 0; x < WIDTH; x++) {
      if (grid[y][x] == 0) {
        iprintf("    ");
      }
      else {
        const char *colorCode;

        switch (grid[y][x]) {
        case 2:
          colorCode = ANSI_DARK_GRAY;
          break;
        case 4:
          colorCode = ANSI_RESET;
          break;
        case 8:
          colorCode = ANSI_DARK_CYAN;
          break;
        case 16:
          colorCode = ANSI_CYAN;
          break;
        case 32:
          colorCode = ANSI_DARK_GREEN;
          break;
        case 64:
          colorCode = ANSI_GREEN;
          break;
        case 128:
          colorCode = ANSI_DARK_YELLOW;
          break;
        case 256:
          colorCode = ANSI_YELLOW;
          break;
        case 512:
          colorCode = ANSI_DARK_RED;
          break;
        case 1024:
          colorCode = ANSI_RED;
          break;
        case 2048:
          colorCode = ANSI_DARK_MAGENTA;
          break;
        case 4096:
          colorCode = ANSI_MAGENTA;
          break;
        default:
          colorCode = ANSI_RESET;
          break; // reset (normal)
        }

        iprintf("%s%4d%s", colorCode, grid[y][x], ANSI_GRAY);
      }
    }

    iprintf("\n\n");
  }

  if (is_game_over) {
    iprintf("\n\n Press start to return to menu  ");
  }
  else if (is_paused) {
    iprintf("\n\n Press again to return to menu  ");
  }
}
