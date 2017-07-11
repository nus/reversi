#include "board.h"
#include "turns.h"
#include <stdio.h>
#include <string.h>

static void reversi_search(const board_t board, phase_t phase, turns_t *turns) {
  board_t hint, next;
  phase_t next_phase;
  int i;
  eb_t eb;
  et_t et;

  board_hint(board, phase, &hint);
  for(i = 0; i < BOARD_LEN; i++) {
    if (hint[i] == CELL_HINT) {
      turns_t *dup = NULL;

      if ((eb = board_put(board, phase, i, next))) {
        fprintf(stderr, "Failed to board_put(): %d\n", eb);
        printf("pahse: %d\n", phase);
        board_print(board);
        board_print(hint);
      } else if ((et = turns_dup(turns, &dup))) {
        fprintf(stderr, "Failed to turns_dup(0x%p): %d\n", turns, et);
      } else if ((et = turns_record(dup, phase, i))) {
        fprintf(stderr, "Failed to turns_record(0x%p, %d, %d): %d\n", turns, phase, i, et);
        turns_delete(dup);
        dup = NULL;
      } else if ((next_phase = board_next_phase(next, phase)) == PHASE_NONE) {
        int black = board_count_cell(next, CELL_BLACK);
        int white = board_count_cell(next, CELL_WHITE);
        int turn = turns_get_count(dup);

        printf("finished(turns: %d, black: %d, white: %d)\n", turn, black, white);
        turns_delete(dup);
        dup = NULL;
      } else {
        reversi_search(next, next_phase, dup);
        turns_delete(dup);
        dup = NULL;
      }
    }
  }
}

int main() {
  turns_t *turns = turns_new();

  reversi_search(BOARD, PHASE_BLACK, turns);
  turns_delete(turns);

  return 0;
}
