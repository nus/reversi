#include "board.h"
#include "test.h"
#include <string.h>
#include <stdio.h>

static int board_cmp(const board_t b1, board_t b2) {
  return memcmp(b1, b2, BOARD_LEN);
}

static void test_board() {
  board_t expected = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  if (board_cmp(BOARD, expected) != 0) {
    fprintf(stderr, "BOARD != expected\n");
    return;
  }
}

static void test_board_reset() {
  board_t b = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 2, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_reset(b);
  if (board_cmp(BOARD, b) != 0) {
    fprintf(stderr, "failed board_cmp\n");
    return;
  }
}

static void test_board_copy() {
  board_t b = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_copy(BOARD, b);
  if (board_cmp(BOARD, b) != 0) {
    fprintf(stderr, "failed board_copy\n");
    return;
  }
}

static void test_board_hint1() {
  board_t hint;

  board_t expected1 = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0,
    0, 0, 3, 2, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 3, 0, 0,
    0, 0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_t expected2 = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 3, 0, 0, 0,
    0, 0, 0, 2, 1, 3, 0, 0,
    0, 0, 3, 1, 2, 0, 0, 0,
    0, 0, 0, 3, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_hint(BOARD, PHASE_BLACK, &hint);
  if (board_cmp(hint, expected1) != 0) {
    fprintf(stderr, "BOARD != expected1\n");
    return;
  }

  board_hint(BOARD, PHASE_WHITE, &hint);
  if (board_cmp(hint, expected2) != 0) {
    fprintf(stderr, "BOARD != expected2\n");
    return;
  }
}

static void test_board_put1() {
  board_t b;
  eb_t e;

  e = board_put(BOARD, (phase_t )3, 0, b);
  if (e != EB_ILLARGS) {
    fprintf(stderr, "Expecting Illegal argument with phase.\n");
    return;
  }

  e = board_put(BOARD, PHASE_BLACK, -1, b);
  if (e != EB_ILLARGS) {
    fprintf(stderr, "Expecting Illegal argument with index.\n");
    return;
  }

  e = board_put(BOARD, PHASE_BLACK, 64, b);
  if (e != EB_ILLARGS) {
    fprintf(stderr, "Expecting Illegal argument with index.\n");
    return;
  }

  e = board_put(BOARD, PHASE_BLACK, 0, b);
  if (e != EB_UNPUTTABLE) {
    fprintf(stderr, "Expecting unputtable.\n");
    return;
  }
}

static void test_board_put2() {
  board_t b;
  eb_t e;

  board_t expected1 = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_t expected2 = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 2, 0, 0, 0,
    0, 0, 0, 2, 2, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  e = board_put(BOARD, PHASE_BLACK, 19, b);
  if (e != EB_OK) {
    fprintf(stderr, "Expecting puttable.\n");
    return;
  }

  if (board_cmp(b, expected1)) {
    fprintf(stderr, "BOARD != expected1\n");
    board_print(b);
    return;
  }

  e = board_put(BOARD, PHASE_WHITE, 20, b);
  if (e != EB_OK) {
    fprintf(stderr, "Expecting puttable.\n");
    return;
  }

  if (board_cmp(b, expected2)) {
    fprintf(stderr, "BOARD != expected2\n");
    board_print(b);
    return;
  }
}

static void test_board_put3() {
  eb_t e;

  board_t b = {
    1, 2, 2, 2, 1, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 2, 0, 2, 2, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 1, 2, 2, 1, 2, 2, 1};

  board_t expected = {
    1, 2, 2, 2, 1, 2, 2, 2,
    2, 1, 2, 2, 1, 2, 2, 1,
    2, 2, 1, 2, 1, 2, 1, 2,
    2, 2, 2, 1, 1, 1, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 1, 1, 1, 2, 2,
    2, 2, 1, 2, 1, 2, 1, 2,
    2, 1, 2, 2, 1, 2, 2, 1};

  e = board_put(b, PHASE_BLACK, 36, b);
  if (e != EB_OK) {
    fprintf(stderr, "Expecting puttable.\n");
    return;
  }

  if (board_cmp(b, expected)) {
    fprintf(stderr, "BOARD != expected\n");
    board_print(b);
    return;
  }
}

static void test_board_put4() {
  eb_t e;

  board_t b = {
    2, 1, 1, 1, 2, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 1, 1, 0, 1, 1, 2,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 1, 2, 1, 1, 2};

  board_t expected = {
    2, 1, 1, 1, 2, 1, 1, 1,
    1, 2, 1, 1, 2, 1, 1, 2,
    1, 1, 2, 1, 2, 1, 2, 1,
    1, 1, 1, 2, 2, 2, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 2, 2, 2, 1, 1,
    1, 1, 2, 1, 2, 1, 2, 1,
    1, 2, 1, 1, 2, 1, 1, 2};

  e = board_put(b, PHASE_WHITE, 36, b);
  if (e != EB_OK) {
    fprintf(stderr, "Expecting puttable.\n");
    return;
  }

  if (board_cmp(b, expected)) {
    fprintf(stderr, "BOARD != expected\n");
    board_print(b);
    return;
  }
}

static void test_board_put5() {
  eb_t e;

  board_t b = {
    0, 0, 2, 1, 2, 2, 2, 2,
    0, 0, 2, 1, 2, 1, 2, 0,
    2, 2, 2, 2, 1, 2, 2, 0,
    2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 0, 0,
    2, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  board_t expected = {
    0, 0, 2, 1, 2, 2, 2, 2,
    0, 0, 2, 1, 2, 1, 1, 1,
    2, 2, 2, 2, 1, 2, 1, 0,
    2, 2, 2, 2, 2, 1, 2, 0,
    0, 2, 2, 2, 1, 2, 0, 0,
    2, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

  e = board_put(b, PHASE_BLACK, 15, b);
  if (e != EB_OK) {
    fprintf(stderr, "Expecting puttable.\n");
    return;
  }

  if (board_cmp(b, expected)) {
    fprintf(stderr, "BOARD != expected\n");
    board_print(b);
    return;
  }
}

static void test_board_count_cell1() {
  board_t b = {
    0, 0, 2, 1, 2, 2, 2, 2,
    0, 0, 2, 1, 2, 1, 2, 0,
    2, 2, 2, 2, 1, 2, 2, 0,
    2, 2, 2, 2, 2, 2, 2, 0,
    0, 2, 2, 2, 2, 2, 0, 0,
    2, 0, 0, 1, 1, 3, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};
  int count = board_count_cell(b, CELL_NONE);

  if (count != 29) {
    fprintf(stderr, "Unexpected none count:%d\n", count);
    return;
  }

  count = board_count_cell(b, CELL_BLACK);
  if (count != 7) {
    fprintf(stderr, "Unexpected black count:%d\n", count);
    return;
  }

  count = board_count_cell(b, CELL_WHITE);
  if (count != 27) {
    fprintf(stderr, "Unexpected white count:%d\n", count);
    return;
  }

  count = board_count_cell(b, CELL_HINT);
  if (count != 1) {
    fprintf(stderr, "Unexpected hint count:%d\n", count);
    return;
  }
}

static void test_board_count_cell2() {
  board_t b;
  int count = board_count_cell(b, 10);

  if (count != -1) {
    fprintf(stderr, "Unexpected count:%d\n", count);
    return;
  }
}

static void test_board_next_phase1() {
  phase_t phase;
  board_t b1 = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};
  board_t b2 = {
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 1, 2, 2, 2, 2,
    2, 2, 1, 2, 2, 2, 2, 2,
    2, 1, 2, 1, 2, 1, 2, 2,
    2, 2, 1, 1, 2, 2, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    1, 2, 2, 1, 2, 2, 2, 2};

  phase = board_next_phase(b1, PHASE_BLACK);
  if (phase != PHASE_WHITE) {
    fprintf(stderr, "Unexpected b1 phase:%d\n", phase);
    return;
  }

  phase = board_next_phase(b2, PHASE_WHITE);
  if (phase != PHASE_NONE) {
    fprintf(stderr, "Unexpected b2 phase:%d\n", phase);
    return;
  }
}

static void test_board_next_phase2() {
  phase_t phase;
  board_t b1 = {
    1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 2, 2, 2, 2,
    0, 0, 2, 2, 1, 2, 2, 2,
    0, 0, 0, 2, 2, 2, 2, 1,
    0, 0, 2, 2, 2, 2, 2, 1,
    0, 0, 0, 2, 2, 2, 2, 1,
    0, 0, 0, 2, 2, 1, 1, 1,
    0, 0, 1, 1, 1, 1, 1, 1};
  board_t b2 = {
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 0,
    2, 2, 2, 2, 2, 2, 0, 0,
    2, 2, 2, 2, 2, 2, 0, 1,
    2, 2, 2, 2, 2, 2, 2, 0,
    2, 2, 2, 2, 2, 2, 2, 2};

  phase = board_next_phase(b1, PHASE_BLACK);
  if (phase != PHASE_BLACK) {
    fprintf(stderr, "Unexpected b1 phase:%d\n", phase);
    return;
  }

  phase = board_next_phase(b2, PHASE_WHITE);
  if (phase != PHASE_NONE) {
    fprintf(stderr, "Unexpected b2 phase:%d\n", phase);
    return;
  }
}


void board_test_main() {
  RUN(test_board);
  RUN(test_board_reset);
  RUN(test_board_copy);
  RUN(test_board_hint1);
  RUN(test_board_put1);
  RUN(test_board_put2);
  RUN(test_board_put3);
  RUN(test_board_put4);
  RUN(test_board_put5);
  RUN(test_board_count_cell1);
  RUN(test_board_count_cell2);
  RUN(test_board_next_phase1);
  RUN(test_board_next_phase2);
}
