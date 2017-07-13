#include "board.h"
#include "turns.h"

typedef struct search_t search_t;
typedef void (*search_on_settled_t)(const board_t, const turns_t *turns);

search_t *search_new(const board_t board, search_on_settled_t on_settled);
void search_delete(search_t *search);
void search_search(search_t *search);
