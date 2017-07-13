#include "search.h"
#include <stdio.h>

static void on_settled(const board_t board, const turns_t *turns) {
  int black = board_count_cell( board, CELL_BLACK);
  int white = board_count_cell( board, CELL_WHITE);
  int turn = turns_get_count(turns);

  printf("finished(turns: %d, black: %d, white: %d)\n", turn, black, white);
}

int main() {
  search_t *search = search_new(BOARD, on_settled);

  search_search(search);
  search_delete(search);

  return 0;
}
