#include <stdio.h>

#include "board.h"
#include "move.h"
#include "engine.h"

void print_options()
{
    printf("***********************************************\n");
    printf("Please run with the following arguments: m d\n");
    printf("\n");
    printf("Meaning of m (mode):\n");
    printf("  0 : player WHITE vs player BLACK\n");
    printf("  1 : player WHITE vs cpu    BLACK\n");
    printf("  2 : cpu    WHITE vs player BLACK\n");
    printf("  3 : cpu    WHITE vs cpu    BLACK\n");
    printf("\n");
    printf("Meaning of d (depth):\n");
    printf("  d is the (positive) numeric evaluation depth for cpu play\n");
    printf("\n");
    printf("Remember, WHITE moves first!\n");
    printf("***********************************************\n");
}

int main(int argc, char *argv[])
{
    square_t *board;

    move_t *moves;
    uint8_t move;

    color_t color;
    uint8_t depth, cpu[2];

    if (argc != 3)
    {
        print_options();
        return 0;
    }

    else
    {
        switch (argv[1][0])
        {
        case '0':
            cpu[0] = 0; /* white */
            cpu[1] = 0; /* black */
            break;

        case '1':
            cpu[0] = 0; /* white */
            cpu[1] = 1; /* black */
            break;

        case '2':
            cpu[0] = 1; /* white */
            cpu[1] = 0; /* black */
            break;

        case '3':
            cpu[0] = 1; /* white */
            cpu[1] = 1; /* black */
            break;

        default:
            print_options();
            return 0;
        }

        depth = atoi(argv[2]);
    }

    board = new_board();
    if (board == NULL)
    {
        /* the function prints a NULL board as a single square with 'NIL' inside */
        print_board(board);
        return -1;
    }

    init_board(board);

    /* game loop */
    color = WHITE;
    while (color != NONE)
    {
        print_board(board);
        moves = list_moves(board, color);
        if (moves != NULL)
        {
            /* list all moves for color */
            printf("%s MOVES:\n", color == WHITE ? "WHITE" : "BLACK");
            print_moves(moves);

            /* choose which move depending of type of player and color */
            move = cpu[color == WHITE ? 0 : 1] ? cpu_choose(board, moves, color, depth) : player_choose(moves);
            printf("-> Move: %u\n", move);

            /* execute move */
            execute_move(board, moves, move);

            /* free memory */
            free_moves(moves);
            moves = NULL;

            /* change player */
            color = color == WHITE ? BLACK : WHITE;
        }
        else
        {
            /* no moves => game over */
            printf("%s IS LOCKED!\n", color == WHITE ? "WHITE" : "BLACK");
            color = NONE;
        }
    }

    /* free memory */
    free_board(board);
    board = NULL;

    return 0;
}
