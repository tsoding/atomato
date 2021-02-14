// Brian's Brain
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "life.h"

#define OFF 0
#define ON 1
#define DYING 2

Uint32 cell_color[3] = {
    [OFF] = BACKGROUND_COLOR,
    [ON] = PINK_COLOR,
    [DYING] = BLUE_COLOR,
};

Cell bb_rule(const Board *prev, int row, int col)
{
    const int nbors = life_board_nbors(prev, row, col, ON);

    switch (prev->cells[row][col]) {
    case OFF:
        return nbors == 2 ? ON : OFF;
    case ON:
        return DYING;
    case DYING:
        return OFF;
    default:
        assert(false && "bb_rule: unreachable");
    }
}

int main(void)
{
    Board board = {0};
    life_random_board(&board, 2);
    life_go(&board, bb_rule, 3, cell_color);
}
