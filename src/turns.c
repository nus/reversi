#include "turns.h"
#include <stdlib.h>
#include <string.h>

#define TURNS_LEN (BOARD_LEN - 4)

typedef struct turns_t {
  phase_t turns[TURNS_LEN];
  int count;
} truns_t;

turns_t *turns_new() {
  return (turns_t *) calloc(sizeof(turns_t), 1);
}

et_t turns_dup(const turns_t *src, turns_t **dup) {
  turns_t *t;

  if (!src) {
    return ET_ILLARGS;
  } else if (!dup) {
    return ET_ILLARGS;
  } else if (*dup) {
    return ET_ILLARGS;
  }

  t = turns_new();
  if (!t) {
    return ET_NOMEM;
  }

  memcpy(t, src, sizeof(turns_t));
  *dup = t;

  return ET_OK;
}

void turns_delete(turns_t *turns) {
  free(turns);
}

et_t turns_record(turns_t *turns, phase_t phase, int index) {
  int phase_bit = 0;

  if (!turns) {
    return ET_ILLARGS;
  } else if (!board_is_valid_phase(phase)) {
    return ET_ILLARGS;
  } else if (!board_is_valid_index(index)) {
    return ET_ILLARGS;
  } else if (turns->count < 0) {
    return ET_ILLSTATE;
  } else if (turns->count >= TURNS_LEN) {
    return ET_ILLSTATE;
  }

  /*
   * phase_bit is 6th digit in binary.
    *
   * |1|2|3|4|5|6|7|8|
   * +---------+-+-+-+
   * |   index |*|-|-|
   * +---------+-+-+-+
   */
  phase_bit = (phase == PHASE_BLACK) ? (0 << 6) : (1 << 6);
  turns->turns[turns->count++] = index + phase_bit;

  return ET_OK;
}

int turns_get_count(const turns_t *turns) {
  if (!turns) {
    return -1;
  }

  return turns->count;
}
