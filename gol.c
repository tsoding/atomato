#include <assert.h>
#include <stdbool.h>
#include "./atomato.h"
#include "./life.h"

#define DED 0
#define ALIVE 1

Uint32 cell_color[2] = {
    [DED] = 0x00000000,
    [ALIVE] = 0xFFAABBFF
};

void render_board(Atomato *context, const Board *board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            atomato_fill_rect(
                context,
                col * CELL_WIDTH,
                row * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
                cell_color[board->cells[row][col]]);
        }
    }
}

Cell gol_rule(const Board *prev, int row, int col)
{
    const int nbors = board_nbors(prev, row, col, ALIVE);

    switch (prev->cells[row][col]) {
    case DED:
        return nbors == 3 ? ALIVE : DED;
    case ALIVE:
        return nbors == 2 || nbors == 3 ? ALIVE : DED;
    default:
        assert(false && "next_board: unreachable");
    }
}

void put_glider_at(Board *board, int row0, int col0)
{
    Cell glider[3][3] = {
        {DED, ALIVE, DED},
        {DED, DED,   ALIVE},
        {ALIVE, ALIVE, ALIVE},
    };

    for (int drow = 0; drow < 3; ++drow) {
        for (int dcol = 0; dcol < 3; ++dcol) {
            const int row = mod(row0 + drow, ROWS);
            const int col = mod(col0 + dcol, COLS);
            board->cells[row][col] = glider[drow][dcol];
        }
    }
}

void init_board(Board *board)
{
    put_glider_at(board, 0, 0);
    put_glider_at(board, 4, 4);
    put_glider_at(board, 8, 8);
}

int main()
{
    return life_event_loop(init_board, gol_rule, render_board);
}
