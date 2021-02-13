// Brian's Brain
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "atomato.h"
#include "life.h"

#define OFF 0
#define ON 1
#define DYING 2

Uint32 cell_color[3] = {
    [OFF] = BACKGROUND_COLOR,
    [ON] = FOREGROUND_COLOR,
    [DYING] = 0xAABBFFFF,
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

Cell bb_rule(const Board *prev, int row, int col)
{
    const int nbors = board_nbors(prev, row, col, ON);

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

void init_board(Board *board)
{
    random_board(board, 2);
}

int main(void)
{
    return life_event_loop(
               init_board,
               bb_rule,
               render_board);
}
