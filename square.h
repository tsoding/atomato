// # Square
//
// One of the Atomato "Frameworks" that provides functionality for
// square grid based cellular automata. Based on the Core framework (see core.h)
//
// # Controls
//
// - G - toggle grid

#ifndef SQUARE_H_
#define SQUARE_H_

#include "./core.h"

#define ROWS 100
#define COLS 100
#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) COLS)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) ROWS)

typedef struct {
    Core core;
    bool cell_clicked;
    int cell_row;
    int cell_col;
    bool grid;
} Square;

void square_begin(Square *context)
{
    core_begin(&context->core);
}

void square_end(Square *context)
{
    core_end(&context->core);
}

bool square_time_to_quit(Square *context)
{
    bool result = core_time_to_quit(&context->core);
    context->cell_row = context->core.mouse_y / CELL_HEIGHT;
    context->cell_col = context->core.mouse_x / CELL_WIDTH;
    context->cell_clicked = context->core.mouse_clicked;

    if (context->core.keyboard['g']) {
        context->grid = !context->grid;
    }

    return result;
}

bool square_is_next_gen(Square *context)
{
    return core_is_next_gen(&context->core);
}

void square_begin_rendering(Square *context)
{
    core_begin_rendering(&context->core);

    if (context->grid) {
        for (int row = 1; row < ROWS; ++row) {
            core_draw_line(
                &context->core,
                0.0, row * CELL_HEIGHT,
                SCREEN_WIDTH, row * CELL_HEIGHT,
                GRID_COLOR);
        }

        for (int col = 1; col < COLS; ++col) {
            core_draw_line(
                &context->core,
                col * CELL_WIDTH, 0.0,
                col * CELL_WIDTH, SCREEN_HEIGHT,
                GRID_COLOR);
        }
    }
}

void square_end_rendering(Square *context)
{
    core_end_rendering(&context->core);
}

void square_fill_cell(Square *context,
                      int row, int col,
                      Uint32 color)
{
    core_fill_rect(
        &context->core,
        col * CELL_WIDTH,
        row * CELL_HEIGHT,
        CELL_WIDTH,
        CELL_HEIGHT,
        color);
}

#endif  // SQUARE_H_
