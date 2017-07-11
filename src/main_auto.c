#include "board.h"
#include <stdio.h>

int main() {
  board_t board, hint, next;
  phase_t phase = PHASE_BLACK;
  int i;

  board_reset(board);
  for (i = 0; i < 70; i++) {
    int k;
    eb_t e;

    board_print(board);

    board_hint(board, phase, &hint);
    for(k = 0; k < BOARD_LEN; k++) {
      if (hint[k] == CELL_HINT) {
        break;
      }
    }

    e = board_put(board, phase, k, next);
    if (e) {
      fprintf(stderr, "Unexpected error: %d\n", e);
      board_print(board);
      board_print(hint);
      break;
    }

    phase = board_next_phase(next, phase);
    if (phase == PHASE_NONE) {
      int black = board_count_cell(next, CELL_BLACK);
      int white = board_count_cell(next, CELL_WHITE);
      printf("finished(turns: %d, black: %d, white: %d)\n", i, black, white);
      board_print(next);
      break;
    }

    board_copy(next, board);
  }

  return 0;
}
