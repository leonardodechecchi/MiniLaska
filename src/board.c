#include "board.h"

#include <stdio.h>
#include <stdlib.h>

/* Square */
#define SQ1WS 64 /* 01000000 */
#define SQ1BS 66 /* 01000010 */
#define SQ000 0  /* 00000000 */

uint8_t get_field(square_t const square, uint8_t const index)
{
    uint8_t mask, shift, field;

    if (3 < index)
        return 0;

    mask = 3; /* 3 -> 00000011 */
    shift = 2 * index;

    field = square & (mask << shift);

    return field >> shift;
}

/* 
00000000 
i primi due bit sono quelli più significati è l'altezza
colonna 6 bit restanti 
*/

uint8_t set_field(square_t const square, uint8_t const index, uint8_t value)
{
    uint8_t mask, mask_0, shift, field;

    if (3 < index || 3 < value)
        return 0;

    mask = 3; /* 3 -> 00000011 */
    shift = 2 * index;

    mask_0 = mask << shift ^ 255; /* 255 -> 11111111 */
    value = value << shift;

    /* Set field to 0, add value and return */
    return square & mask_0 | value;
}

/* Converts the tuple (row, col) to a single index value. Row, col are expected to be in [0, 7) to return a value in [0, 25) */
uint8_t toi(uint8_t const row, uint8_t const col)
{
    /* vedere i miei appunti per questa formula */
    return (7 * row + col + 8) / 2 - 4;
}

/*===== HEADER FUNCTIONS ========================================================*/

square_t *new_board()
{
    square_t *board = (square_t *)malloc(sizeof(square_t) * 25);
    return board;
}

void free_board(square_t *board)
{
    if (board != NULL)
        free(board);
}

void copy_board(square_t const *board_1, square_t *board_2)
{
    uint8_t i;

    if (board_1 == NULL || board_2 == NULL)
        return;

    for (i = 0; i < 25; ++i)
        board_2[i] = board_1[i];
}

void init_board(square_t *board)
{
    uint8_t row, col;

    if (board == NULL)
        return;

    for (row = 0; row < 7; ++row)
    {
        for (col = row % 2; col < 7; col += 2)
        {
            /* WHITE */
            if (row < 3)
                board[toi(row, col)] = SQ1WS;

            /* BLACK */
            else if (row > 3)
                board[toi(row, col)] = SQ1BS;

            /* EMPTY */
            else
                board[toi(row, col)] = SQ000;
        }
    }
}

uint8_t num_pieces(square_t const *board, uint8_t const row, uint8_t const col)
{
    uint8_t i = toi(row, col);
    return get_field(board[i], 3);
}

piece_t top_piece(square_t const *board, uint8_t const row, uint8_t const col)
{
    uint8_t num, i;

    i = toi(row, col);
    num = get_field(board[i], 3);

    return num > 0 ? get_field(board[i], num - 1) : NONE;
}

rank_t get_piece(square_t const *board, uint8_t const row, uint8_t const col, uint8_t const height)
{
    uint8_t num, i;

    i = toi(row, col);
    num = get_field(board[i], 3);

    return num > height ? get_field(board[i], height) : NONE;
}

color_t color_of(piece_t const piece)
{
    /* left bit, 2 -> 00000010 */
    return piece == NONE ? NONE : piece & 2;
}

piece_t rank_of(piece_t const piece)
{
    /* right bit, 1 -> 00000001 */
    return piece == NONE ? NONE : piece & 1;
}

uint8_t valid(uint8_t const row, uint8_t const col)
{
    return row < 7 && col < 7 && row % 2 == col % 2;
}

void move(square_t *board, uint8_t const row_1, uint8_t const col_1, uint8_t const row_2, uint8_t const col_2)
{
    uint8_t i, j;

    i = toi(row_1, col_1);
    j = toi(row_2, col_2);

    board[j] = board[i];
    board[i] = SQ000;
}

void eat(square_t *board, uint8_t const row_1, uint8_t const col_1, uint8_t const row_2, uint8_t const col_2)
{
    uint8_t i, num;
    piece_t piece;

    if (!valid(row_1, col_1) || !valid(row_2, col_2))
        return;

    /* let's get top piece of column 1 */
    i = toi(row_1, col_1);
    num = get_field(board[i], 3);

    if (num == 0)
        return;

    piece = get_field(board[i], num - 1);

    /* changing the num field 'shrinks' the column by 1 */
    board[i] = set_field(board[i], 3, num - 1);

    /* now let's push back to column 2 */
    i = toi(row_2, col_2);
    num = get_field(board[i], 3);

    if (num < 3)
    {
        /* moving all fields to the left */
        /* remember, they are [num (3), p2 (2), p1 (1), p0 (0)] */
        board[i] = board[i] << 2;
        board[i] = set_field(board[i], 3, num + 1);
    }

    /* If height == 3, bottom piece is automatically overwritten */
    board[i] = set_field(board[i], 0, piece);
}

void promote(square_t *board, uint8_t const row, uint8_t const col)
{
    uint8_t num, i;

    i = toi(row, col);
    num = get_field(board[i], 3);

    if (num > 0)
        board[i] = set_field(board[i], num - 1, get_field(board[i], num - 1) | GENERAL);
}

void print_board(square_t const *board)
{
    uint8_t row, col, height, i;
    piece_t piece;

    if (board == NULL)
    {
        printf("  +-------+\n");
        printf("  |       |\n");
        printf("  |  NIL  |\n");
        printf("  |       |\n");
        printf("  +-------+\n");
        return;
    }

    printf("      A       B       C       D       E       F       G    \n");

    /* 7 to 1 */
    for (row = 7; row > 0; --row)
    {
        printf("  +-------+-------+-------+-------+-------+-------+-------+\n");

        /* 3 to 1 */
        for (height = 3; height > 0; --height)
        {
            if (height == 2)
                printf("%d |", row);

            else
                printf("  |");

            /* 1 to 7 */
            for (col = 1; col <= 7; ++col)
            {
                /* conversion: 0 to 6 */
                if (!valid(row - 1, col - 1))
                    printf("       ");

                else
                {
                    /* conversion: 0 to 6 */
                    piece = get_piece(board, row - 1, col - 1, height - 1);

                    switch (piece)
                    {
                    case WHITE_SOLDIER:
                        printf("[=====]");
                        break;

                    case WHITE_GENERAL:
                        printf("[=(*)=]");
                        break;

                    case BLACK_SOLDIER:
                        printf("[#####]");
                        break;

                    case BLACK_GENERAL:
                        printf("[#{*}#]");
                        break;

                    default: /* case NONE: */
                        printf(" . . . ");
                        break;
                    }
                }

                printf("|");
            }

            if (height == 2)
                printf(" %d\n", row);

            else
                printf("\n");
        }
    }
    printf("  +-------+-------+-------+-------+-------+-------+-------+\n");
    printf("      A       B       C       D       E       F       G    \n");
}