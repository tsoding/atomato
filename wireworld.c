// https://en.wikipedia.org/wiki/Wireworld

#include <assert.h>
#include "./life.h"

typedef enum {
    EMPTY = 0,
    HEAD,
    TAIL,
    CONDUCTOR,
    CELL_COUNT
} Wireworld_Cell;

Color cell_color[CELL_COUNT] = {
    [EMPTY] = BACKGROUND_COLOR,
    [HEAD] = BLUE_COLOR,
    [TAIL] = PINK_COLOR,
    [CONDUCTOR] = YELLOW_COLOR
};

Cell wireworld_rule(const Board *prev, int row, int col)
{
    const int nbor_heads = life_board_nbors(prev, row, col, HEAD);

    switch (prev->cells[row][col]) {
    case EMPTY:
        return EMPTY;
    case HEAD:
        return TAIL;
    case TAIL:
        return CONDUCTOR;
    case CONDUCTOR:
        return (nbor_heads == 1 || nbor_heads == 2) ? HEAD : CONDUCTOR;
    default:
        assert(false && "wireworld_rule: unreachable");
    }
}

int main()
{
    Board board = {0};

    life_go(&board,
            wireworld_rule,
            CELL_COUNT,
            cell_color);

    return 0;
}
