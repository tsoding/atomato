// https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)

#include "./atomato.h"
#include "./life.h"

#define OFF 0
#define ON 1

Uint32 cell_color[2] = {
    [OFF] = BACKGROUND_COLOR,
    [ON] = FOREGROUND_COLOR
};

Cell seeds_rule(const Board *prev, int row, int col)
{
    const int nbors = board_nbors(prev, row, col, ON);
    if (prev->cells[row][col] == OFF && nbors == 2) {
        return ON;
    } else {
        return OFF;
    }
}

void render_board(const Board *board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            atomato_fill_rect(
                col * CELL_WIDTH,
                row * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
                cell_color[board->cells[row][col]]);
        }
    }
}

void put_glider_at(Board *board, int row0, int col0)
{
    static const Cell glider[4][2] = {
        {ON, OFF},
        {OFF, ON},
        {OFF, ON},
        {ON, OFF},
    };

    for (int drow = 0; drow < 4; ++drow) {
        for (int dcol = 0; dcol < 2; ++dcol) {
            const int row = mod(row0 + drow, ROWS);
            const int col = mod(col0 + dcol, COLS);
            board->cells[row][col] = glider[drow][dcol];
        }
    }
}

void init_board(Board *board)
{
    for (int i = 0; i < 10; ++i) {
        put_glider_at(board, i * 4, i * 4);
    }
}

int main(void)
{
    return life_event_loop(
               init_board,
               seeds_rule,
               render_board);
}
