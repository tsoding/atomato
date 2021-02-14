// Here we keep the common code for all of the Cellular Atomata
// from the Game of Life Family:
// https://en.wikipedia.org/wiki/Life-like_cellular_automaton

#ifndef LIFE_H_
#define LIFE_H_

#include "square.h"

typedef int Cell;

typedef struct {
    Cell cells[ROWS][COLS];
} Board;

int board_nbors(const Board *board, int row0, int col0, Cell cell)
{
    int result = 0;
    for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
            if (drow != 0 || dcol != 0) {
                const int row = mod(row0 + drow, ROWS);
                const int col = mod(col0 + dcol, COLS);

                if (board->cells[row][col] == cell) {
                    result += 1;
                }
            }
        }
    }
    return result;
}

void next_board(const Board *prev, Board *next,
                int (*rule)(const Board *prev, int row, int col))
{
    if (rule) {
        for (int row = 0; row < ROWS; ++row) {
            for (int col = 0; col < COLS; ++col) {
                next->cells[row][col] = rule(prev, row, col);
            }
        }
    }
}

void random_board(Board *board, int cell_state)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board->cells[row][col] = rand() % cell_state;
        }
    }
}

Board board[2] = {0};
int fg = 0;

int life_event_loop(void (*init_board)(Board *board),
                    Cell (*rule)(const Board *prev, int row, int col),
                    void (*render_board)(Core *context, const Board *board))
{
    Core context = {0};

    if (init_board) {
        init_board(&board[fg]);
    }

    core_begin(&context);

    while (!core_time_to_quit(&context)) {
        if (context.mouse_clicked) {
            const int col = (int) floorf(context.mouse_x / CELL_WIDTH);
            const int row = (int) floorf(context.mouse_y / CELL_HEIGHT);

            // TODO: life_event_callback assumes that there is only two kinds of cells (0 and 1)
            board[fg].cells[row][col] = 1 - board[fg].cells[row][col];
        }

        // TODO: life framework does not clean the screen on R anymore
        // I removed it during refactoring

        if (core_is_next_gen(&context)) {
            const int bg = 1 - fg;
            next_board(&board[fg], &board[bg], rule);
            fg = bg;
        }

        core_begin_rendering(&context);
        {
            if (render_board) {
                render_board(&context, &board[fg]);
            }
        }
        core_end_rendering(&context);
    }

    core_end(&context);

    return 0;
}

#endif // LIFE_H_
