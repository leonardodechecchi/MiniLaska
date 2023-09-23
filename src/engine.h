#ifndef STRATEGY_H
#define STRATEGY_H

#include "bigint.h"
#include "board.h"
#include "move.h"

/* Waits for and returns numeric user input as soon as it is in [0, upper). */
uint8_t player_choose(move_t const *moves);

/* Returns the index number of the 'best choice' from the given moves */
/* board cannot be NULL, moves can be NULL, color can be {WHITE, BLACK} */
uint8_t cpu_choose(square_t const *board, move_t const *moves, color_t const color, uint8_t const max_depth);

#endif /* STRATEGY_H */