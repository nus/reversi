#ifndef BOARD_H
#define BOARD_H

typedef enum {
  EB_OK,
  EB_ILLARGS, // Illegal arguments
  EB_UNPUTTABLE
} eb_t;

typedef char cell_t;
#define CELL_NONE   ((cell_t) 0)
#define CELL_BLACK  ((cell_t) 1)
#define CELL_WHITE  ((cell_t) 2)
#define CELL_HINT   ((cell_t) 3)

#define EDGE (8)
#define BOARD_LEN (EDGE * EDGE)
/**
 *    a  b  c  d  e  f  g  h
 *  +------------------------
 * 1| 0  1  2  3  4  5  6  7
 * 2| 8  9  10 11 12 13 14 15
 * 3| 16 17 18 19 20 21 22 23
 * 4| 24 25 26 27 28 29 30 31
 * 5| 32 33 34 35 36 37 38 39
 * 6| 40 41 42 43 44 45 46 47
 * 7| 48 49 50 51 52 53 54 55
 * 8| 56 57 58 59 60 61 62 63
 */
typedef cell_t board_t[BOARD_LEN];
extern const board_t BOARD;

#define CELL_INDEX(alphabet, numeric) (alphabet + numeric * EDGE)

typedef char phase_t;
#define PHASE_BLACK ((phase_t) 1)
#define PHASE_WHITE ((phase_t) 2)
#define PHASE_NONE  ((phase_t) 3)

/**
 * Reset board as BOARD.
 */
void board_reset(board_t board);

/**
 * Copy board
 */
void board_copy(const board_t src, board_t dst);

/**
 * Print board to stdout.
 */
void board_print(const board_t board);

/**
 * @return 0 if it is puttable. 1 if it is not puttable.
 */
int board_is_puttable(const board_t board, phase_t phase, int index);

/**
 * @args dst mark as CELL_HINT on board.
 */
void board_hint(const board_t board, phase_t phase, board_t *dst);

/**
 * @return EB_OK, EB_ILLARGS, EB_UNPUTTABLE
 */
eb_t board_put(const board_t board, phase_t phase, int index, board_t dst);

/**
 * Count cells on the board.
 * @return Number of cells on the board. -1 if unexpected cell.
 */
int board_count_cell(const board_t board, cell_t cell);

/**
 * Choose next phase.
 * @return PHASE_BLACK, PHASE_WHITE, PHASE_NONE
 */
phase_t board_next_phase(const board_t board, phase_t current);

/**
 * Validate a phase
 * @return This returns 1 if phase is valid. 0 if phase is not OK.
 */
int board_is_valid_phase(phase_t phase);

/**
 * Validate a index
 * @return This returns 1 if index is valid. 0 if index is out of boad_t range.
 */
int board_is_valid_index(int index);

#endif // BOARD_H
