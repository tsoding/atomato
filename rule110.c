#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./square.h"

typedef enum {
    O = 0,
    I,
    COUNT_CELL
} Rule110_Cell;

Uint32 cell_color[COUNT_CELL] = {
    [O] = 0,
    [I] = PINK_COLOR,
};

#define PATTERN_SIZE 3

#define PATTERN(A, B, C) ((A << 2) | (B << 1) | C)

Rule110_Cell patterns[1 << PATTERN_SIZE] = {
    [PATTERN(O, O, O)] = O,
    [PATTERN(O, O, I)] = I,
    [PATTERN(O, I, O)] = I,
    [PATTERN(O, I, I)] = I,
    [PATTERN(I, O, O)] = O,
    [PATTERN(I, O, I)] = I,
    [PATTERN(I, I, O)] = I,
    [PATTERN(I, I, I)] = O,
};

typedef struct {
    Rule110_Cell cells[COLS];
} Row;

Row next_row(Row prev)
{
    Row next = {0};

    // N = 5
    // 01234
    // *****
    //  ^

    for (int i = 0; i < COLS; ++i) {
        const int index = PATTERN(prev.cells[mod(i - 1, COLS)],
                                  prev.cells[i],
                                  prev.cells[mod(i + 1, COLS)]);
        next.cells[i] = patterns[index];
    }

    return next;
}

Row random_row(void)
{
    Row result = {0};

    for (int i = 0; i < COLS; ++i) {
        result.cells[i] = rand() % 2;
    }

    return result;
}

typedef struct {
    Row rows[ROWS];
    int begin;
    int size;
} Board;

Board board = {0};

void board_push_row(Board *board, Row row)
{
    board->rows[mod(board->begin + board->size, ROWS)] = row;

    if (board->size < ROWS) {
        board->size += 1;
    } else {
        board->begin = mod(board->begin + 1, ROWS);
    }
}

void board_next_row(Board *board)
{
    board_push_row(board, next_row(board->rows[mod(board->begin + board->size - 1, ROWS)]));
}

void board_render(Square *context, const Board *board)
{
    for (int row = 0; row < board->size; ++row) {
        for (int col = 0; col < COLS; ++col) {
            square_fill_cell(
                context, row, col,
                cell_color[board->rows[mod(board->begin + row, ROWS)].cells[col]]);
        }
    }
}

int main(void)
{
    Square context = {0};

    square_begin(&context);

    board_push_row(&board, random_row());

    while (!square_time_to_quit(&context)) {
        // Update State
        const size_t count = square_next_gen_count(&context);
        for (size_t i = 0; i < count; ++i) {
            board_next_row(&board);
        }

        // Render State
        square_begin_rendering(&context);
        {
            board_render(&context, &board);
        }
        square_end_rendering(&context);
    }

    square_end(&context);

    return 0;
}
