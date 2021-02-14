#include <assert.h>
#include <stdbool.h>
#include "./life.h"

typedef enum {
    DED = 0,
    ALIVE,
    CELL_COUNT
} Gol_Cell;

Color cell_color[CELL_COUNT] = {
    [DED] = 0,
    [ALIVE] = PINK_COLOR
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

    life_go(&board, gol_rule, CELL_COUNT, cell_color, "gol.png");

    return 0;
}
