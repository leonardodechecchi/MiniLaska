#include "move.h"

#include <stdio.h>

/* A row, column position */
typedef struct position
{
    uint8_t row;
    uint8_t col;
} position_t;

position_t compute_position(uint8_t const row, uint8_t const col, uint8_t const distance, uint8_t const direction)
{
    position_t position;

    switch (direction)
    {
    case 0: /* NE */
        position.row = row + distance;
        position.col = col + distance;
        break;

    case 1: /* NO */
        position.row = row + distance;
        position.col = col - distance;
        break;

    case 2: /* SE */
        position.row = row - distance;
        position.col = col + distance;
        break;

    case 3: /* SO */
        position.row = row - distance;
        position.col = col - distance;
        break;

    default:
        position.row = row;
        position.col = col;
        break;
    }

    return position;
}

move_t *prepend_move(move_t *moves, uint8_t const row_1, uint8_t const col_1, uint8_t const row_2, uint8_t const col_2, uint8_t eating)
{
    move_t *tmp = (move_t *)malloc(sizeof(move_t));

    if (tmp == NULL)
        return moves;

    tmp->row_1 = row_1;
    tmp->col_1 = col_1;

    tmp->row_2 = row_2;
    tmp->col_2 = col_2;

    tmp->eating = eating;

    tmp->next = moves;
    return tmp;
}

/*===== HEADER FUNCTIONS ========================================================*/

move_t *list_moves(square_t const *board, color_t const color)
{
    move_t *normal, *eating;

    uint8_t row_1, col_1, direction, is_eating;

    piece_t p_1;
    rank_t r_1;
    color_t c_1, c_2;

    position_t pos_2;

    eating = NULL;
    normal = NULL;

    /* iterate over all squares */
    for (row_1 = 0; row_1 < 7; ++row_1)
    {
        for (col_1 = row_1 % 2; col_1 < 7; col_1 += 2)
        {
            p_1 = top_piece(board, row_1, col_1);
            c_1 = color_of(p_1);
            r_1 = rank_of(p_1);

            if (c_1 != color)
                continue;

            is_eating = 0;
            for (direction = 0; direction < 4; ++direction)
            {
                /* conditions for which directions to check */
                if (r_1 != GENERAL && !(c_1 == WHITE && direction <= 1) && !(c_1 == BLACK && 2 <= direction))
                    continue;

                pos_2 = compute_position(row_1, col_1, 1, direction);
                if (!valid(pos_2.row, pos_2.col))
                    continue;

                c_2 = color_of(top_piece(board, pos_2.row, pos_2.col));

                if (c_2 == NONE)
                {
                    if (!is_eating)
                        normal = prepend_move(normal, row_1, col_1, pos_2.row, pos_2.col, 0);
                }

                else if (c_2 != c_1)
                {
                    pos_2 = compute_position(row_1, col_1, 2, direction);
                    if (!valid(pos_2.row, pos_2.col))
                        continue;

                    c_2 = color_of(top_piece(board, pos_2.row, pos_2.col));

                    if (c_2 == NONE)
                    {
                        eating = prepend_move(eating, row_1, col_1, pos_2.row, pos_2.col, 1);
                        is_eating = 1;
                    }
                }
            }
        }
    }

    /* always prefer eating moves to normal ones */
    if (eating != NULL)
    {
        if (normal != NULL)
            free_moves(normal);

        return eating;
    }

    return normal;
}

void free_moves(move_t *moves)
{
    move_t *tmp;

    while (moves != NULL)
    {
        tmp = moves;
        moves = moves->next;
        free(tmp);
    }
}

void execute_move(square_t *board, move_t const *moves, uint8_t const index)
{
    uint8_t row_1, col_1, row_2, col_2, i;
    piece_t p;

    if (moves == NULL)
        return;

    i = 0;
    while (i < index && moves != NULL)
    {
        moves = moves->next;
        ++i;
    }

    if (moves == NULL)
        return;

    row_1 = moves->row_1;
    col_1 = moves->col_1;

    row_2 = moves->row_2;
    col_2 = moves->col_2;

    /* actual execution */
    move(board, row_1, col_1, row_2, col_2);
    if (moves->eating)
        eat(board, (row_1 + row_2) / 2, (col_1 + col_2) / 2, row_2, col_2);

    /* eventually, promote */
    p = top_piece(board, row_2, col_2);
    if (rank_of(p) == SOLDIER && (color_of(p) == WHITE && row_2 == 6 || color_of(p) == BLACK && row_2 == 0))
        promote(board, row_2, col_2);
}

void print_moves(move_t const *moves)
{
    uint8_t i;

    if (moves == NULL)
    {
        printf("> NIL\n");
        return;
    }

    i = 0;
    while (moves != NULL)
    {
        printf("[%d] %c%d %c> %c%d\n",
               i,
               moves->col_1 + 65, moves->row_1 + 1,
               moves->eating ? '*' : '-',
               moves->col_2 + 65, moves->row_2 + 1);

        moves = moves->next;
        ++i;
    }
}