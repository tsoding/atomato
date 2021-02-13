#ifndef ATOMATO_H_
#define ATOMATO_H_

#include <stdbool.h>
#include <SDL.h>

int scc(int code)
{
    if (code < 0) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    return code;
}

void *scp(void *ptr)
{
    if (ptr == NULL) {
        fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
        exit(1);
    }

    return ptr;
}

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 1024
#define SCREEN_FPS 60
#define DELTA_TIME_SEC (1.0f / SCREEN_FPS)
#define DELTA_TIME_MS ((int) floorf(DELTA_TIME_SEC * 1000))

#define BACKGROUND_COLOR 0x181818ff
#define FOREGROUND_COLOR 0xFFAABBFF
#define HEX_COLOR_UNPACK(color) \
  ((color >> (8 * 3)) & 0xFF),  \
  ((color >> (8 * 2)) & 0xFF),  \
  ((color >> (8 * 1)) & 0xFF),  \
  ((color >> (8 * 0)) & 0xFF)

#define ROWS 100
#define COLS 100
#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) COLS)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) ROWS)

#define NEXT_GEN_TIMEOUT 0.05f

typedef struct {
    bool quit;
    bool pause;
    float next_gen_timeout;
    SDL_Window *window;
    SDL_Renderer *renderer;
    float mouse_x;
    float mouse_y;
} Atomato;

void atomato_fill_rect(Atomato *context,
                       float x, float y,
                       float w, float h,
                       Uint32 color)
{
    const SDL_Rect rect = {
        .x = (int) floorf(x),
        .y = (int) floorf(y),
        .w = (int) floorf(w),
        .h = (int) floorf(h)
    };

    scc(SDL_SetRenderDrawColor(
            context->renderer,
            HEX_COLOR_UNPACK(color)));

    scc(SDL_RenderFillRect(context->renderer, &rect));
}

void atomato_begin(Atomato *context)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    context->window = scp(SDL_CreateWindow(
                              "Atomato",
                              0, 0,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_RESIZABLE));

    context->renderer =
        scp(SDL_CreateRenderer(context->window,
                               -1,
                               SDL_RENDERER_ACCELERATED));

    scc(SDL_RenderSetLogicalSize(context->renderer,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT));

    scc(SDL_SetRenderDrawBlendMode(
            context->renderer,
            SDL_BLENDMODE_BLEND));
}

void atomato_end(Atomato *context)
{
    (void) context;
    SDL_Quit();
}

bool atomato_time_to_quit(const Atomato *context)
{
    return context->quit;
}

void atomato_begin_rendering(Atomato *context)
{
    scc(SDL_SetRenderDrawColor(
            context->renderer,
            HEX_COLOR_UNPACK(BACKGROUND_COLOR)));
    scc(SDL_RenderClear(context->renderer));
}

#define WITH_ALPHA(color, alpha) ((color & 0xFFFFFF00) | alpha)

#define PAUSE_PADDING 50.0f
#define PAUSE_BAR_WIDTH 20.0f
#define PAUSE_BAR_HEIGHT 100.0f
#define PAUSE_BAR_GAP 20.0f
#define PAUSE_BAR_COLOR 0xFF0000FF
#define PAUSE_WIDTH (2.0f * PAUSE_BAR_WIDTH + PAUSE_BAR_GAP)
#define PAUSE_HEIGHT PAUSE_BAR_HEIGHT

void atomato_draw_pause_symbol(Atomato *context, float x, float y)
{
    Uint32 color = PAUSE_BAR_COLOR;

    if (x <= context->mouse_x &&
            context->mouse_x <= x + PAUSE_WIDTH &&
            y <= context->mouse_y &&
            context->mouse_y <= y + PAUSE_HEIGHT) {
        color = WITH_ALPHA(color, 150);
    }

    atomato_fill_rect(
        context,
        x, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);

    atomato_fill_rect(
        context,
        x + PAUSE_BAR_GAP + PAUSE_BAR_WIDTH, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);
}

void atomato_end_rendering(Atomato *context)
{
    if (context->pause) {
        atomato_draw_pause_symbol(context, PAUSE_PADDING, PAUSE_PADDING);
    }

    SDL_RenderPresent(context->renderer);
    SDL_Delay(DELTA_TIME_MS);
}

// TODO: remove atomato_poll_events
void atomato_poll_events(Atomato *context, void (*callback)(const SDL_Event *event))
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT: {
            context->quit = true;
        }
        break;

        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_SPACE: {
                context->pause = !context->pause;
            }
            break;
            }
        }
        break;

        case SDL_MOUSEMOTION: {
            context->mouse_x = event.motion.x;
            context->mouse_y = event.motion.y;
        }
        break;
        }

        if (callback) {
            callback(&event);
        }
    }

    if (context->next_gen_timeout <= 0.0) {
        context->next_gen_timeout = NEXT_GEN_TIMEOUT;
    }

    if (!context->pause) {
        context->next_gen_timeout -= DELTA_TIME_SEC;
    }
}

bool atomato_is_next_gen(Atomato *context)
{
    return context->next_gen_timeout <= 0.0f;
}

int mod(int a, int b)
{
    return (a % b + b) % b;
}

#endif  // ATOMATO_H_
