#include <assert.h>
#include <stdbool.h>
#include "./core.h"
#include "./life.h"

#define DED 0
#define ALIVE 1

Uint32 cell_color[2] = {
    [DED] = 0x00000000,
    [ALIVE] = 0xFFAABBFF
};

Cell gol_rule(const Board *prev, int row, int col)
{
    const int nbors = life_board_nbors(prev, row, col, ALIVE);

    switch (prev->cells[row][col]) {
    case DED:
        return nbors == 3 ? ALIVE : DED;
    case ALIVE:
        return nbors == 2 || nbors == 3 ? ALIVE : DED;
    default:
        assert(false && "next_board: unreachable");
    }
}

static const Cell glider[3][3] = {
    {DED,   ALIVE,   DED},
    {DED,     DED, ALIVE},
    {ALIVE, ALIVE, ALIVE},
};

int main()
{
    Board board = {0};
    life_copy_shape_to(&board, 0, 0, 3, 3, glider);
    life_copy_shape_to(&board, 4, 4, 3, 3, glider);
    life_copy_shape_to(&board, 8, 8, 3, 3, glider);

    life_go(&board, gol_rule, 2, cell_color);

    return 0;
}
