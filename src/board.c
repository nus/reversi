#include "board.h"
#include <stdio.h>
#include <string.h>

/* privates */
#define DIR_UP (-EDGE)
#define DIR_UP_RIGHT (-EDGE + 1)
#define DIR_RIGHT (1)
#define DIR_DOWN_RIGHT (EDGE + 1)
#define DIR_DOWN (EDGE)
#define DIR_DOWN_LEFT (EDGE - 1)
#define DIR_LEFT (-1)
#define DIR_UP_LEFT  (-EDGE - 1)

#define DIRS_LEN (8)
static const  int DIRS[DIRS_LEN] = {
    DIR_UP,
    DIR_UP_RIGHT,
    DIR_RIGHT,
    DIR_DOWN_RIGHT,
    DIR_DOWN,
    DIR_DOWN_LEFT,
    DIR_LEFT,
    DIR_UP_LEFT
  };

static int cell_is_ok(cell_t cell) {
  switch (cell) {
    case CELL_NONE:
    case CELL_BLACK:
    case CELL_WHITE:
    case CELL_HINT:
      return 1;
    default:
      return 0;
  }
}

static int board_is_puttable_dir(const board_t board, phase_t phase, int index, int dir) {
  int i = 0;
  int next = index;

  for (i = 0; i < EDGE; i++) {
    int mod = (next % EDGE);
    cell_t cell;

    if (mod == 0) {
      switch (dir) {
        case DIR_DOWN_LEFT:
        case DIR_LEFT:
        case DIR_UP_LEFT:
          return 0;
      }
    } else if (mod == 7) {
      switch (dir) {
        case DIR_UP_RIGHT:
        case DIR_RIGHT:
        case DIR_DOWN_RIGHT:
          return 0;
      }
    }

    next += dir;
    if (next < 0 || next >= BOARD_LEN) {
      break;
    }

    cell = board[next];
    if (cell == CELL_NONE) {
      break;
    }

    if (i == 0) {
      if (phase == PHASE_BLACK) {
        if (cell == CELL_BLACK) {
          break;
        }
      } else { /* phase == PHASE_WHITE */
        if (cell == CELL_WHITE) {
          break;
        }
      }
    }

    if (phase == PHASE_BLACK) {
      if (cell == CELL_WHITE) {
        continue;
      } else {
        return 1;
      }
    } else { /* phase == PHASE_WHITE */
      if (cell == CELL_BLACK) {
        continue;
      } else {
        return 1;
      }
    }
  }

  return 0;
}

/* public */

/**
 * 0  1  2  3  4  5  6  7
 * 8  9  10 11 12 13 14 15
 * 16 17 18 19 20 21 22 23
 * 24 25 26 27 28 29 30 31
 * 32 33 34 35 36 37 38 39
 * 40 41 42 43 44 45 46 47
 * 48 49 50 51 52 53 54 55
 * 56 57 58 59 60 61 62 63
 */
const board_t BOARD = {
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 2, 1, 0, 0, 0,
  0, 0, 0, 1, 2, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0};

void board_reset(board_t board) {
  board_copy(BOARD, board);
}

void board_copy(const board_t src, board_t dst) {
  memcpy(dst, src, BOARD_LEN);
}

void board_print(const board_t board) {
  int i, k;

  puts("   a b c d e f g h");
  puts("+-----------------");

  for (i = 0; i < EDGE; i++) {
    printf("%d| ", i + 1);
    for (k = 0; k < EDGE; k++) {
      cell_t c = board[CELL_INDEX(k, i)];
      if (c == CELL_HINT) {
        printf("x ");
      } else {
        printf("%d ", c);
      }
    }
    putchar('\n');
  }
}

int board_is_puttable(const board_t board, phase_t phase, int index) {
  int i;

  if (board[index] != CELL_NONE) {
    return 0;
  }

  for (i = 0; i < DIRS_LEN; i++) {
    int dir = DIRS[i];

    if (board_is_puttable_dir(board, phase, index, dir)) {
      return 1;
    }
  }

  return 0;
}

void board_hint(const board_t board, phase_t phase, board_t *dst) {
  int i, k;

  for (i = 0; i < EDGE; i++) {
    for (k = 0; k < EDGE; k++) {
      int index = CELL_INDEX(k, i);
      cell_t c = board[index];

      if (board_is_puttable(board, phase, index)) {
        (*dst)[index] = CELL_HINT;
      } else {
        (*dst)[index] = c;
      }
    }
  }
}

eb_t board_put(const board_t board, phase_t phase, int index, board_t dst) {
  int i;

  if (dst == NULL) {
    return EB_ILLARGS;
  } else if (!board_is_valid_phase(phase)) {
    return EB_ILLARGS;
  } else if (!board_is_valid_index(index)) {
    return EB_ILLARGS;
  }

  if (!board_is_puttable(board, phase, index)) {
    return EB_UNPUTTABLE;
  }

  /* copy */
  board_copy(board, dst);

  /* reverse */
  for (i = 0; i < DIRS_LEN; i++) {
    int dir = DIRS[i];

    if (board_is_puttable_dir(dst, phase, index, dir)) {
      int k;
      int next = index;

      dst[index] = (phase == PHASE_BLACK) ? CELL_BLACK : CELL_WHITE;
      for (k = 0; k < EDGE; k++) {
        next += dir;

        if (phase == PHASE_BLACK) {
          if (dst[next] == CELL_BLACK) {
            break;
          }

          dst[next] = CELL_BLACK;
        } else { /* phase == PHASE_WHITE */
          if (dst[next] == CELL_WHITE) {
            break;
          }

          dst[next] = CELL_WHITE;
        }
      }
    }
  }

  return EB_OK;
}

int board_count_cell(const board_t board, cell_t cell) {
  int i, count = 0;
  if (!cell_is_ok(cell)) {
    return -1;
  }

  for (i = 0; i < BOARD_LEN; i++) {
    if (board[i] == cell) {
      count++;
    }
  }

  return count;
}

phase_t board_next_phase(const board_t board, phase_t current) {
  board_t hint;
  int candidates;

  current = (current == PHASE_BLACK) ? PHASE_WHITE : PHASE_BLACK;
  board_hint(board, current, &hint);
  candidates = board_count_cell(hint, CELL_HINT);
  if (candidates > 0) {
    return current;
  }

  current = (current == PHASE_BLACK) ? PHASE_WHITE : PHASE_BLACK;
  board_hint(board, current, &hint);
  candidates = board_count_cell(hint, CELL_HINT);
  if (candidates > 0) {
    return current;
  } else {
    return PHASE_NONE;
  }
}

int board_is_valid_phase(phase_t phase) {
  if (phase == PHASE_BLACK) {
    return 1;
  } else if (phase == PHASE_WHITE) {
    return 1;
  } else {
    return 0;
  }
}

int board_is_valid_index(int index) {
  return ((index >= 0) && (index < BOARD_LEN));
}
