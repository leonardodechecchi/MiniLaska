#include "engine.h"

#include <stdio.h>
#include <stdlib.h>

bigint_t get_score(square_t const *board, color_t const color)
{
    uint8_t row, col, num, i;

    piece_t p;
    color_t c;
    rank_t r;

    int8_t numeric_superiority; /* total control on the board (color - opponent) */
    int8_t column_superiority;  /* how many columns I have (color - opponent) */
    int8_t rank_superiority;    /* how many generals I have (color - opponent) */

    bigint_t score = {0, 0, 0, 0};

    if (board == NULL)
        return score;

    numeric_superiority = 0;
    column_superiority = 0;
    rank_superiority = 0;

    for (row = 0; row < 7; ++row)
    {
        for (col = row % 2; col < 7; col += 2)
        {
            num = num_pieces(board, row, col);

            for (i = 0; i < num; ++i)
            {
                p = get_piece(board, row, col, i);
                if (p == NONE)
                    continue;

                c = color_of(p);
                r = rank_of(p);

                if (c == color)
                {
                    ++numeric_superiority;

                    if (i == num - 1)
                    {
                        if (num > 2)
                            ++column_superiority;

                        if (r == GENERAL)
                            ++rank_superiority;
                    }
                }

                else if (c != NONE)
                {
                    if (i == num - 1)
                    {
                        --numeric_superiority;

                        if (num > 2)
                            --column_superiority;

                        if (r == GENERAL)
                            --rank_superiority;
                    }
                }
            }
        }
    }

    /* assemble those three in a single number, in decreasing order of importance */
    score = from_digits(numeric_superiority, column_superiority, rank_superiority);
    return score;
}

typedef struct choice
{
    /* game score associated with choice */
    bigint_t score;

    /* numeric index of the move (from the moves list) */
    uint8_t move;
} choice_t;

/* Finds move that minimizes the score of color */
choice_t minimize(square_t const *board, move_t const *moves, color_t const color, uint8_t const depth, bigint_t alpha, bigint_t beta);

/* Finds move that maximizes the score of color */
choice_t maximize(square_t const *board, move_t const *moves, color_t const color, uint8_t const depth, bigint_t alpha, bigint_t beta);

choice_t minimize(square_t const *board, move_t const *moves, color_t const color, uint8_t const depth, bigint_t alpha, bigint_t beta)
{
    move_t *available;
    square_t *clone;
    uint8_t i;
    choice_t min, best;

    /* either a terminal state OR reached maximum simulation depth */
    if (moves == NULL || depth == 0)
    {
        min.score = get_score(board, color); /* score is relevant */
        min.move = 0;                        /* choice is not relevant */

        return min;
    }

    /* default return choice (worst possible case) */
    min.score = positive_infinity();
    min.move = 0; /* choice is not relevant */

    /* allocate a board to be modified later */
    clone = new_board();

    /* if cannot allocate => cannot simulate, thus this branch is to be avoided (because unknown) */
    if (clone == NULL)
        return min;

    /* for each move, evaluate that branch */
    i = 0;
    while (moves != NULL)
    {
        copy_board(board, clone);

        /* execute current move on that clone */
        execute_move(clone, moves, i);

        /* get all moves available to color and MAXIMIZE */
        available = list_moves(clone, color);
        best = maximize(clone, available, color, depth - 1, alpha, beta);
        if (available != NULL)
            free_moves(available);

        /* resulting score must be a new minimum */
        if (less_than(best.score, min.score))
        {
            /* setting the score */
            min.score = best.score;

            /* and the choice */
            min.move = i;
        }

        if (less_than(min.score, alpha) || equals(min.score, alpha))
            break;

        if (less_than(min.score, beta))
            beta = min.score;

        /* finally passing on next move */
        moves = moves->next;
        ++i;
    }

    /* free memory */
    free_board(clone);
    return min;
}

choice_t maximize(square_t const *board, move_t const *moves, color_t const color, uint8_t const depth, bigint_t alpha, bigint_t beta)
{
    move_t *available;
    square_t *clone;
    uint8_t i;
    choice_t max, best;

    /* either a terminal state OR reached maximum simulation depth */
    if (moves == NULL || depth == 0)
    {
        max.score = get_score(board, color); /* score is relevant */
        max.move = 0;                        /* choice is not relevant */

        return max;
    }

    /* default return value (worst possible case) */
    max.score = negative_infinity();
    max.move = 0; /* choice is not relevant */

    /* allocate a board to be modified later */
    clone = new_board();

    /* if cannot allocate => cannot simulate, thus this branch is to be avoided (because unknown) */
    if (clone == NULL)
        return max;

    /* for each move, evaluate that branch */
    i = 0;
    while (moves != NULL)
    {
        /* clone the original board */
        copy_board(board, clone);

        /* execute current move on that clone */
        execute_move(clone, moves, i);

        /* get all moves available to color and MINIMIZE */
        available = list_moves(clone, color);
        best = minimize(clone, available, color, depth - 1, alpha, beta);
        if (available != NULL)
            free_moves(available);

        /* resulting score must be a new maximum */
        if (greater_than(best.score, max.score))
        {
            /* setting the score */
            max.score = best.score;

            /* and the choice */
            max.move = i;
        }

        if (greater_than(max.score, beta) || equals(max.score, beta))
            break;

        if (greater_than(max.score, alpha))
            alpha = max.score;

        /* finally passing on next move */
        moves = moves->next;
        ++i;
    }

    free_board(clone);
    return max;
}

/*===== HEADER FUNCTIONS ========================================================*/

uint8_t cpu_choose(square_t const *board, move_t const *moves, color_t const color, uint8_t const max_depth)
{
    printf("Thinking...\n");
    return maximize(board, moves, color, max_depth, negative_infinity(), positive_infinity()).move;
}

uint8_t player_choose(move_t const *moves)
{
    uint8_t dim, choice, chosen;
    char str[8];

    dim = 0;
    while (moves != NULL)
    {
        moves = moves->next;
        ++dim;
    }

    chosen = 0;
    while (!chosen)
    {
        printf("Which one?: ");
        fgets(str, 8, stdin);
        choice = atoi(str);

        if (choice < dim)
            chosen = 1;

        else
            printf("NO!\n");
    }

    return choice;
}
