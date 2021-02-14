// # Core
//
// One of the Atomato "Frameworks" that provides the core functionality:
// - creating window
// - organizing event loop
// - providing rendering capabilities
// - synchronizing next generation loop
//
// It does not assume any specific cellular atomaton or the grid it's living on.
//
// ## Usage
//
// ```c
// #include "./core.h"
//
// int main(void)
// {
//     Core context = {0};
//     core_begin(&context);
//
//     while (!core_time_to_quit(&context)) {
//         if (core_is_next_gen(&context)) {
//             // ... compute next generation ...
//         }
//
//         core_begin_rendering(&context);
//         // ... render your generation on the screen ...
//         core_end_rendering(&context);
//     }
//
//     core_end(&context);
//     return 0;
// }
// ```
//
// ## Controls
//
// - SPACE - toggle pause for the "Next Gen" tick

#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>
#include <stdlib.h>
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

typedef Uint32 Color;

#define BACKGROUND_COLOR 0x181818ff
#define FOREGROUND_COLOR 0xFFAABBFF
#define HEX_COLOR_UNPACK(color) \
  ((color >> (8 * 3)) & 0xFF),  \
  ((color >> (8 * 2)) & 0xFF),  \
  ((color >> (8 * 1)) & 0xFF),  \
  ((color >> (8 * 0)) & 0xFF)

#define NEXT_GEN_TIMEOUT 0.05f

typedef struct {
    bool quit;
    bool pause;
    float next_gen_timeout;
    SDL_Window *window;
    SDL_Renderer *renderer;
    float mouse_x;
    float mouse_y;
    bool mouse_clicked;
} Core;

void core_fill_rect(Core *context,
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

void core_begin(Core *context)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    context->window = scp(SDL_CreateWindow(
                              "Core",
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

void core_end(Core *context)
{
    (void) context;
    SDL_Quit();
}

bool core_time_to_quit(Core *context)
{
    context->mouse_clicked = false;

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

        case SDL_MOUSEBUTTONDOWN: {
            context->mouse_clicked = true;
            context->mouse_x = event.motion.x;
            context->mouse_y = event.motion.y;
        }
        break;
        }
    }

    if (context->next_gen_timeout <= 0.0) {
        context->next_gen_timeout = NEXT_GEN_TIMEOUT;
    }

    if (!context->pause) {
        context->next_gen_timeout -= DELTA_TIME_SEC;
    }

    return context->quit;
}

void core_begin_rendering(Core *context)
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

void core_draw_pause_symbol(Core *context, float x, float y)
{
    Uint32 color = PAUSE_BAR_COLOR;

    if (x <= context->mouse_x &&
            context->mouse_x <= x + PAUSE_WIDTH &&
            y <= context->mouse_y &&
            context->mouse_y <= y + PAUSE_HEIGHT) {
        color = WITH_ALPHA(color, 150);
    }

    core_fill_rect(
        context,
        x, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);

    core_fill_rect(
        context,
        x + PAUSE_BAR_GAP + PAUSE_BAR_WIDTH, y,
        PAUSE_BAR_WIDTH,
        PAUSE_BAR_HEIGHT,
        color);
}

void core_end_rendering(Core *context)
{
    if (context->pause) {
        core_draw_pause_symbol(context, PAUSE_PADDING, PAUSE_PADDING);
    }

    SDL_RenderPresent(context->renderer);
    SDL_Delay(DELTA_TIME_MS);
}

bool core_is_next_gen(Core *context)
{
    return context->next_gen_timeout <= 0.0f;
}

int mod(int a, int b)
{
    return (a % b + b) % b;
}

#endif  // CORE_H_
