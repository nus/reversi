#include "board.h"
#include "libui/ui.h"
#include <stdio.h>
#include <string.h>

static int to_index(char alphabet, char numeric) {
  if ((alphabet < 'a') || (alphabet > 'h')) {
    return -1;
  } else if ((numeric < '0') || (numeric > '8')) {
    return -1;
  }

  return ((numeric - '1') * EDGE) + (alphabet - 'a');
}

int main() {
  phase_t phase = PHASE_BLACK;
  board_t board;
  board_t next;

  memcpy(board, BOARD, BOARD_LEN);

  while (1) {
    int r = 0;
    char alphabet = -1;
    char numeric = -1;
    int index = -1;
    eb_t e = EB_OK;

    board_print(board);
    printf(((phase == PHASE_BLACK) ? "黒の番:" : "白の番:"));

    r = scanf("%c%c", &alphabet, &numeric);
    while (getchar() != '\n'); /* for flush the stdin buffer */
    if (!r) {
      continue;
    } else if ((alphabet == -1) || (numeric == -1)) {
      continue;
    }

    index = to_index(alphabet, numeric);
    if (index == -1) {
      continue;
    }

    e = board_put(board, phase, index, next);
    if (e) {
      continue;
    }

    memcpy(board, next, BOARD_LEN);

    phase = (phase == PHASE_BLACK) ? PHASE_WHITE : PHASE_BLACK;
  }

  return 0;
}
