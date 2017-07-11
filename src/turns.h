#ifndef TURNS_H
#define TURNS_H

#include "board.h"

typedef enum {
  ET_OK,
  ET_ILLARGS,   /* Illegal arguments */
  ET_ILLSTATE,  /* Illegal state */
  ET_NOMEM,     /* No memory */
} et_t;

typedef struct turns_t turns_t;

/**
 * Create a turns_t instance.
 */
turns_t *turns_new();

/**
 *
 */
et_t turns_dup(const turns_t *src, turns_t **dup);

/**
 * Delete the turns_t instance.
 */
void turns_delete(turns_t *turns);

/**
 * Record a turn.
 * @return ET_OK, ET_ILLARGS, ET_ILLSTATE
 */
et_t turns_record(turns_t *turns, phase_t phase, int index);

/**
 * Count of turns.
 * @return Count of turns. Returns -1 if invalid argument.
 */
int turns_get_count(const turns_t *turns);

#endif // TURNS_H
