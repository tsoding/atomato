// https://en.wikipedia.org/wiki/Wireworld

#include <assert.h>
#include <string.h>
#include <errno.h>
#include "./life.h"

typedef enum {
    EMPTY = 0,
    HEAD,
    TAIL,
    CONDUCTOR,
    CELL_COUNT
} Wireworld_Cell;

Color cell_color[CELL_COUNT] = {
    [EMPTY] = 0,
    [HEAD] = 0xFFFFFFFF,
    [TAIL] = 0x0080FFFF,
    [CONDUCTOR] = 0xFF8000FF,
};

Color parse_color[CELL_COUNT] = {
    // [EMPTY] = 0,
    // [HEAD] = BLUE_COLOR,
    // [TAIL] = PINK_COLOR,
    // [CONDUCTOR] = YELLOW_COLOR
    [EMPTY] = 0xFF000000,
    [HEAD] = 0xFFFFFFFF,
    [TAIL] = 0xFFFF8000,
    [CONDUCTOR] = 0xFF0080FF,
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

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "USAGE: ./wireworld <image.gif>\n");
        fprintf(stderr, "ERROR: no input is provided\n");
        exit(1);
    }

    Board board = {0};

    life_load_board_from_image(&board, argv[1], CELL_COUNT, parse_color);

    life_go(&board,
            wireworld_rule,
            CELL_COUNT,
            cell_color,
            "wireworld.png");

    return 0;
}
