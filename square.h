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

// TODO: settings size of the board at runtime
#define ROWS 600
#define COLS 800
#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) COLS)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) ROWS)

typedef struct {
    float scale;
    V2 pos;
} Camera;

V2 camera_to_screen(const Camera *camera, V2 world)
{
    const V2 half_screen = v2_scale((V2) {
        SCREEN_WIDTH, SCREEN_HEIGHT
    }, 0.5f);

    return v2_add(
               v2_scale(
                   v2_add(world, camera->pos),
                   camera->scale),
               half_screen);
}

V2 camera_to_world(const Camera *camera, V2 screen)
{
    const V2 half_screen = v2_scale((V2) {
        SCREEN_WIDTH, SCREEN_HEIGHT
    }, 0.5f);

    return v2_sub(
               v2_scale(
                   v2_sub(screen, half_screen),
                   1.0f / camera->scale),
               camera->pos);
}

Camera default_camera(void)
{
    return (Camera) {
        .pos = {0},
        .scale = 1.0f,
    };
}

typedef struct {
    Core core;
    bool cell_clicked;
    int cell_row;
    int cell_col;
    // TODO: square grid is not scaled with the camera properly
    // It would be probably better make the grid a texture as well
    bool grid;
    SDL_Texture *board_texture;
    void *board_pixels;
    int board_pitch;
    Camera camera;
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

    context->camera = default_camera();
}

void square_end(Square *context)
{
    SDL_DestroyTexture(context->board_texture);
    core_end(&context->core);
}

float clampf(float v, float lo, float hi)
{
    return fminf(fmaxf(lo, v), hi);
}

bool square_time_to_quit(Square *context)
{
    bool result = core_time_to_quit(&context->core);

    const V2 half_screen = v2_scale((V2) {
        SCREEN_WIDTH, SCREEN_HEIGHT
    }, 0.5f);

    const V2 world_mouse =
        v2_add(
            camera_to_world(&context->camera, context->core.mouse),
            half_screen);

    context->cell_row = world_mouse.y / CELL_HEIGHT;
    context->cell_col = world_mouse.x / CELL_WIDTH;
    context->cell_clicked = context->core.mouse_clicked;

    if (context->core.keyboard['g']) {
        context->grid = !context->grid;
    }

    // TODO: use different scale quality on different zoom levels
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    if (context->core.mouse_wheel_y != 0) {
        context->camera.scale =
            clampf(context->camera.scale + context->core.mouse_wheel_y * 0.25f * context->camera.scale,
                   0.005f,
                   100.0f);
    }

    if (context->core.keyboard['0']) {
        context->camera = default_camera();
    }

    if (context->core.mouse_pressed) {
        const V2 d = v2_sub(context->core.mouse, context->core.prev_mouse);
        context->camera.pos =
            v2_add(
                context->camera.pos,
                v2_scale(d, 1.0f / context->camera.scale));
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

    V2 pos = camera_to_screen(
                 &context->camera,
                 v2(SCREEN_WIDTH * -0.5f, SCREEN_HEIGHT * -0.5f));
    V2 size = v2_scale(
                  v2(SCREEN_WIDTH, SCREEN_HEIGHT),
                  context->camera.scale);

    const SDL_Rect srcrect = {0, 0, COLS, ROWS};
    const SDL_Rect dstrect = {
        (int) floorf(pos.x),
        (int) floorf(pos.y),
        (int) floorf(size.x),
        (int) floorf(size.y)
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
