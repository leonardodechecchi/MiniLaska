#ifndef MOVE_H
#define MOVE_H

#include <stdlib.h>
#include "board.h"

/* A list of moves */
typedef struct move
{
    uint8_t row_1, col_1;
    uint8_t row_2, col_2;

    uint8_t eating;

    struct move *next;
} move_t;

/* Returns a list of all possible moves for color. If color can eat => it must do that */
move_t *list_moves(square_t const *board, color_t const color);

/* Executes the move at 'index' from the list, modifying board */
void execute_move(square_t *board, move_t const *moves, uint8_t const index);

/* Frees the whole list of moves */
void free_moves(move_t *moves);

/* Prints the list of moves to stdout. moves can be NULL */
void print_moves(move_t const *moves);

#endif /* MOVE_H */