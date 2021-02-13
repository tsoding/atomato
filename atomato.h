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

Atomato global = {0};

SDL_Window *atomato_create_window(void)
{
    return scp(SDL_CreateWindow(
                   "Atomato",
                   0, 0,
                   SCREEN_WIDTH, SCREEN_HEIGHT,
                   SDL_WINDOW_RESIZABLE));
}

SDL_Renderer *atomato_create_renderer(SDL_Window *window)
{
    SDL_Renderer * renderer =
        scp(SDL_CreateRenderer(window,
                               -1,
                               SDL_RENDERER_ACCELERATED));

    scc(SDL_RenderSetLogicalSize(renderer,
                                 SCREEN_WIDTH,
                                 SCREEN_HEIGHT));

    return renderer;
}

void atomato_fill_rect(float x, float y, float w, float h, Uint32 color)
{
    const SDL_Rect rect = {
        .x = (int) floorf(x),
        .y = (int) floorf(y),
        .w = (int) floorf(w),
        .h = (int) floorf(h)
    };

    scc(SDL_SetRenderDrawColor(
            global.renderer,
            HEX_COLOR_UNPACK(color)));

    scc(SDL_RenderFillRect(global.renderer, &rect));
}

void atomato_begin(void)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    global.window = atomato_create_window();
    global.renderer = atomato_create_renderer(global.window);

    scc(SDL_SetRenderDrawBlendMode(
            global.renderer,
            SDL_BLENDMODE_BLEND));
}

void atomato_end(void)
{
    SDL_Quit();
}

bool atomato_time_to_quit(void)
{
    return global.quit;
}

void atomato_begin_rendering(void)
{
    scc(SDL_SetRenderDrawColor(
            global.renderer,
            HEX_COLOR_UNPACK(BACKGROUND_COLOR)));
    scc(SDL_RenderClear(global.renderer));
}

#define WITH_ALPHA(color, alpha) ((color & 0xFFFFFF00) | alpha)

#define PAUSE_PADDING 50.0f
#define PAUSE_BAR_WIDTH 20.0f
#define PAUSE_BAR_HEIGHT 100.0f
#define PAUSE_BAR_GAP 20.0f
#define PAUSE_BAR_COLOR 0xFF0000FF
#define PAUSE_WIDTH (2.0f * PAUSE_BAR_WIDTH + PAUSE_BAR_GAP)
#define PAUSE_HEIGHT PAUSE_BAR_HEIGHT

void atomato_draw_pause_symbol(float x, float y)
{
    Uint32 color = PAUSE_BAR_COLOR;

    if (x <= global.mouse_x &&
            global.mouse_x <= x + PAUSE_WIDTH &&
            y <= global.mouse_y &&
            global.mouse_y <= y + PAUSE_HEIGHT) {
        color = WITH_ALPHA(color, 150);
    }

    atomato_fill_rect(
        x, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);

    atomato_fill_rect(
        x + PAUSE_BAR_GAP + PAUSE_BAR_WIDTH, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);
}

void atomato_end_rendering(void)
{
    if (global.pause) {
        atomato_draw_pause_symbol(PAUSE_PADDING, PAUSE_PADDING);
    }

    SDL_RenderPresent(global.renderer);
    SDL_Delay(DELTA_TIME_MS);
}

void atomato_poll_events(void (*callback)(const SDL_Event *event))
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT: {
            global.quit = true;
        }
        break;

        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
            case SDLK_SPACE: {
                global.pause = !global.pause;
            }
            break;
            }
        }
        break;

        case SDL_MOUSEMOTION: {
            global.mouse_x = event.motion.x;
            global.mouse_y = event.motion.y;
        }
        break;
        }

        if (callback) {
            callback(&event);
        }
    }

    if (global.next_gen_timeout <= 0.0) {
        global.next_gen_timeout = NEXT_GEN_TIMEOUT;
    }

    if (!global.pause) {
        global.next_gen_timeout -= DELTA_TIME_SEC;
    }
}

bool atomato_is_next_gen(void)
{
    return global.next_gen_timeout <= 0.0f;
}

int mod(int a, int b)
{
    return (a % b + b) % b;
}

#endif  // ATOMATO_H_
