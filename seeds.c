// https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)

#include "./core.h"
#include "./life.h"

typedef enum {
    OFF = 0,
    ON,
    CELL_COUNT
} Seeds_Cell;

Color cell_color[CELL_COUNT] = {
    [OFF] = 0,
    [ON] = PINK_COLOR
};

Cell seeds_rule(const Board *prev, int row, int col)
{
    const int nbors = life_board_nbors(prev, row, col, ON);
    if (prev->cells[row][col] == OFF && nbors == 2) {
        return ON;
    } else {
        return OFF;
    }
}

static const Cell glider[4][2] = {
    {ON, OFF},
    {OFF, ON},
    {OFF, ON},
    {ON, OFF},
};

int main(void)
{
    // TODO: seeds does not read it's initial state from an image file

    Board board = {0};

    for (int i = 0; i < 10; ++i) {
        life_copy_shape_to(
            &board,
            i * 4, i * 4,
            4, 2,
            glider);
    }

    life_go(&board, seeds_rule, CELL_COUNT, cell_color, "seeds.png");
}
