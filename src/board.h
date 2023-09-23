#ifndef BOARD_H
#define BOARD_H

#include "bigint.h"

/* Color */
#define WHITE 0 /* 00000000 */
#define BLACK 2 /* 00000010 */

/* Rank */
#define SOLDIER 0 /* 00000000 */
#define GENERAL 1 /* 00000001 */

/* Piece */
#define WHITE_SOLDIER 0 /* 00000000 */
#define WHITE_GENERAL 1 /* 00000001 */
#define BLACK_SOLDIER 2 /* 00000010 */
#define BLACK_GENERAL 3 /* 00000011 */

/* Null value */
#define NONE 255 /* 11111111 */

typedef unsigned char rank_t;
typedef unsigned char color_t;
typedef unsigned char piece_t;
typedef unsigned char square_t;

/* Returns a heap-allocated board as an array of 25 square_t. The board is NOT initialized! */
/* Returns NULL if failed allocation. Programmer MUST use free_board() to delete the board. */
square_t *new_board();

/* Frees board */
void free_board(square_t *board);

/* Copies all square values from board_1 to board_2 */
void copy_board(square_t const *board_1, square_t *board_2);

/* Initializes board to a fresh game state (11 pieces WHITE at bottom, 11 pieces BLACK at top) */
void init_board(square_t *board);

/* Prints the full board configuration to stdout. board can be NULL */
void print_board(square_t const *board);

/* Returns the number of pieces in row, col */
uint8_t num_pieces(square_t const *board, uint8_t const row, uint8_t const col);

/* Returns the topmost piece in row, col */
piece_t top_piece(square_t const *board, uint8_t const row, uint8_t const col);

/* Returns the piece at specified height in row, col */
piece_t get_piece(square_t const *board, uint8_t const row, uint8_t const col, uint8_t const height);

/* Returns the color of the given piece */
color_t color_of(piece_t const piece);

/* Returns the rank of the given piece */
rank_t rank_of(piece_t const piece);

/* Returns 1 if the tuple (row, col) is one of the 25 playable positions, 0 otherwise. */
uint8_t valid(uint8_t const row, uint8_t const col);

/* Moves all pieces from position 1 to position 2 */
void move(square_t *board, uint8_t const row_1, uint8_t const col_1, uint8_t const row_2, uint8_t const col_2);

/* Moves the top piece at position 1 below all pieces at position 2 */
/* If there are 3 pieces at position 2 => the existing bottom piece is permanently removed from the game */
void eat(square_t *board, uint8_t const row_1, uint8_t const col_1, uint8_t const row_2, uint8_t const col_2);

/* The topmost piece at row, col is promoted from soldier to general */
void promote(square_t *board, uint8_t const row, uint8_t const col);

#endif /* BOARD_H */