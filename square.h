// # Square
//
// One of the Atomato "Frameworks" that provides functionality for
// square grid based cellular automata. Based on the Core framework (see core.h)

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
    return result;
}

bool square_is_next_gen(Square *context)
{
    return core_is_next_gen(&context->core);
}

void square_begin_rendering(Square *context)
{
    core_begin_rendering(&context->core);
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
