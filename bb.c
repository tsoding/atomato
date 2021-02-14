// Brian's Brain
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "core.h"
#include "life.h"

typedef enum {
    OFF = 0,
    ON,
    DYING,
    CELL_COUNT
} Bb_Cell;

Uint32 cell_color[CELL_COUNT] = {
    [OFF] = 0,
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
    life_go(&board, bb_rule, CELL_COUNT, cell_color);
    return 0;
}
