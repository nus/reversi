#include "search.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct search_t {
  board_t board;
  search_on_settled_t on_settled;
} search_t;


static void reversi_search(const board_t board, phase_t phase, turns_t *turns, search_on_settled_t on_settled) {
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
        on_settled(next, dup);
        turns_delete(dup);
        dup = NULL;
      } else {
        reversi_search(next, next_phase, dup, on_settled);
        turns_delete(dup);
        dup = NULL;
      }
    }
  }
}

/* public */
search_t *search_new(const board_t board, search_on_settled_t on_settled) {
  search_t *search;
  if (!board) {
    return NULL;
  } else if (!on_settled) {
    return NULL;
  }

  search = (search_t *) malloc(sizeof(search_t));
  if (!search) {
    return NULL;
  }

  board_copy(board, search->board);
  search->on_settled = on_settled;

  return search;
}

void search_delete(search_t *search) {
  if (search) {
    free(search);
  }
}

void search_search(search_t *search) {
  turns_t *turns;

  if (!search) {
    fprintf(stderr, "search must be not NULL.\n");
    return;
  }

  turns = turns_new();
  if (!turns) {
    fprintf(stderr, "Faield to turns_new().\n");
    return;
  }

  reversi_search(search->board, PHASE_BLACK, turns, search->on_settled);
  turns_delete(turns);
}
