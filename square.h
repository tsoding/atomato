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

#define ROWS 600
#define COLS 800
#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) COLS)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) ROWS)

typedef struct {
    Core core;
    bool cell_clicked;
    int cell_row;
    int cell_col;
    bool grid;
    SDL_Texture *board_texture;
    void *board_pixels;
    int board_pitch;
    float camera_scale;
    float camera_x;
    float camera_y;
} Square;

void square_begin(Square *context)
{
    core_begin(&context->core);

    context->board_texture =
        scp(SDL_CreateTexture(
                context->core.renderer,
                SDL_PIXELFORMAT_RGBA32,
                SDL_TEXTUREACCESS_STREAMING,
                COLS,
                ROWS));

    scc(SDL_SetTextureBlendMode(context->board_texture,
                                SDL_BLENDMODE_BLEND));

    context->camera_scale = 1.0;
    context->camera_x = 0;
    context->camera_y = 0;
}

void square_end(Square *context)
{
    SDL_DestroyTexture(context->board_texture);
    core_end(&context->core);
}

bool square_time_to_quit(Square *context)
{
    bool result = core_time_to_quit(&context->core);
    // TODO: square framework does not take into account camera when mapping clickes on cells
    context->cell_row = context->core.mouse_y / CELL_HEIGHT;
    context->cell_col = context->core.mouse_x / CELL_WIDTH;
    context->cell_clicked = context->core.mouse_clicked;

    if (context->core.keyboard['g']) {
        context->grid = !context->grid;
    }

    if (context->core.keyboard['=']) {
        context->camera_scale = context->camera_scale * 1.1;
    }

    if (context->core.keyboard['-']) {
        context->camera_scale = fmaxf(context->camera_scale / 1.1, 0.005);
    }

    if (context->core.keyboard['i']) {
        context->camera_y += 10.0 * context->camera_scale;
    }

    if (context->core.keyboard['k']) {
        context->camera_y -= 10.0 * context->camera_scale;
    }

    if (context->core.keyboard['j']) {
        context->camera_x += 10.0 * context->camera_scale;
    }

    if (context->core.keyboard['l']) {
        context->camera_x -= 10.0 * context->camera_scale;
    }

    if (context->core.keyboard['0']) {
        context->camera_x = 0.0;
        context->camera_y = 0.0;
        context->camera_scale = 1.0;
    }

    return result;
}

bool square_next_gen_count(Square *context)
{
    return core_next_gen_count(&context->core);
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

    scc(SDL_LockTexture(
            context->board_texture,
            NULL,
            &context->board_pixels,
            &context->board_pitch));
}

void square_end_rendering(Square *context)
{
    SDL_UnlockTexture(context->board_texture);

    const float w = SCREEN_WIDTH * context->camera_scale;
    const float h = SCREEN_HEIGHT * context->camera_scale;
    const float x = SCREEN_WIDTH * 0.5 - w * 0.5 + context->camera_x;
    const float y = SCREEN_HEIGHT * 0.5 - h * 0.5 + context->camera_y;

    const SDL_Rect srcrect = {0, 0, COLS, ROWS};
    const SDL_Rect dstrect = {
        (int) floorf(x),
        (int) floorf(y),
        (int) floorf(w),
        (int) floorf(h)
    };

    scc(SDL_RenderCopy(context->core.renderer,
                       context->board_texture,
                       &srcrect,
                       &dstrect));

    core_end_rendering(&context->core);
}

void square_fill_cell(Square *context,
                      int row, int col,
                      Uint32 color)
{
    Uint8 *pixels = context->board_pixels;
    *((Uint32 *)&pixels[row * context->board_pitch + col * 4]) = color;
}

#endif  // SQUARE_H_
