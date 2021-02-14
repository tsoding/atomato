// https://en.wikipedia.org/wiki/Wireworld

#include <assert.h>
#include <string.h>
#include <errno.h>
#include "./life.h"

#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"

typedef enum {
    EMPTY = 0,
    HEAD,
    TAIL,
    CONDUCTOR,
    CELL_COUNT
} Wireworld_Cell;

Color cell_color[CELL_COUNT] = {
    // [EMPTY] = 0,
    // [HEAD] = BLUE_COLOR,
    // [TAIL] = PINK_COLOR,
    // [CONDUCTOR] = YELLOW_COLOR
    [EMPTY] = 0,
    [HEAD] = 0xFFFFFFFF,
    [TAIL] = 0x0080FFFF,
    [CONDUCTOR] = 0xFF8000FF,

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

    const char *filename = argv[1];
    int x, y, n;

    uint32_t *data = (uint32_t *) stbi_load(filename, &x, &y, &n, 0);

    assert(n == 4);
    assert(COLS >= x);
    assert(ROWS >= y);

    if (data != NULL) {
        printf("filename = %s, x = %d, y = %d, n = %d\n", filename, x, y, n);
    } else {
        fprintf(stderr, "ERROR: could not read file %s: %s",
                filename, strerror(errno));
        exit(1);
    }

    Board board = {0};

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            switch (data[row * x + col]) {
            case 0xFF000000:
                board.cells[row][col] = EMPTY;
                break;
            case 0xFF0080FF:
                board.cells[row][col] = CONDUCTOR;
                break;
            case 0xFFFFFFFF:
                board.cells[row][col] = HEAD;
                break;
            case 0xFFFF8000:
                board.cells[row][col] = TAIL;
                break;
            default: {
                fprintf(stderr, "Unexpected color 0x%08X\n",
                        data[row * COLS + col]);
                exit(1);
            }
            }
        }
    }

    life_go(&board,
            wireworld_rule,
            CELL_COUNT,
            cell_color);

    return 0;
}
