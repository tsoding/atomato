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
// - X - speed up the gen tick
// - Z - slow down the gen tick

#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL.h>

#include "./style.h"

typedef struct {
    float x, y;
} V2;

V2 v2(float x, float y)
{
    return (V2) {
        x, y
    };
}

V2 v2_add(V2 a, V2 b)
{
    return (V2) {
        a.x + b.x, a.y + b.y
    };
}

V2 v2_sub(V2 a, V2 b)
{
    return (V2) {
        a.x - b.x, a.y - b.y
    };
}

V2 v2_scale(V2 a, float s)
{
    return (V2) {
        a.x * s, a.y * s
    };
}

float v2_len(V2 a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}

float v2_dist(V2 a, V2 b)
{
    return v2_len(v2_sub(b, a));
}

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


#define NEXT_GEN_MIN_TIMEOUT 0.0000000005
#define NEXT_GEN_INITIAL_TIMEOUT NEXT_GEN_MIN_TIMEOUT // 0.05f
#define CLICK_THRESHOLD_DISTANCE 10.0f

typedef struct {
    bool quit;
    bool pause;
    float next_gen_cooldown;
    float next_gen_timeout;
    size_t next_gen_count;
    SDL_Window *window;
    SDL_Renderer *renderer;
    V2 mouse;
    V2 prev_mouse;
    V2 drag_mouse;
    Sint32 mouse_wheel_y;
    bool mouse_pressed;
    bool mouse_clicked;
    bool keyboard[256];
} Core;

void core_draw_line(Core *context,
                    float x1, float y1,
                    float x2, float y2,
                    Color color)
{
    scc(SDL_SetRenderDrawColor(
            context->renderer,
            RGBA32_UNPACK(color)));

    scc(SDL_RenderDrawLine(context->renderer,
                           (int) floorf(x1),
                           (int) floorf(y1),
                           (int) floorf(x2),
                           (int) floorf(y2)));
}

void core_fill_rect(Core *context,
                    float x, float y,
                    float w, float h,
                    Uint32 color)
{
    const SDL_Rect rect = {
        .x = (int) ceilf(x),
        .y = (int) ceilf(y),
        .w = (int) ceilf(w),
        .h = (int) ceilf(h)
    };

    scc(SDL_SetRenderDrawColor(
            context->renderer,
            RGBA32_UNPACK(color)));

    scc(SDL_RenderFillRect(context->renderer, &rect));
}

void core_begin(Core *context)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    context->next_gen_timeout = NEXT_GEN_INITIAL_TIMEOUT;

    context->window = scp(SDL_CreateWindow(
                              "Core",
                              0, 0,
                              SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_RESIZABLE));

    context->renderer =
        scp(SDL_CreateRenderer(context->window,
                               -1,
                               SDL_RENDERER_ACCELERATED));

    // TODO: logical size cuts of the texture of square framework
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
    memset(context->keyboard, 0, sizeof(context->keyboard));

    context->prev_mouse = context->mouse;

    context->mouse_wheel_y = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT: {
            context->quit = true;
        }
        break;

        case SDL_KEYDOWN: {
            int sym = event.key.keysym.sym;
            switch (sym) {
            case SDLK_SPACE: {
                context->pause = !context->pause;
            }
            break;

            case SDLK_x: {
                context->next_gen_timeout = fmaxf(context->next_gen_timeout / 1.5,
                                                  NEXT_GEN_MIN_TIMEOUT);
                context->next_gen_cooldown = context->next_gen_timeout;
            }
            break;

            case SDLK_z: {
                context->next_gen_timeout = context->next_gen_timeout * 1.5;
                context->next_gen_cooldown = context->next_gen_timeout;
            }
            break;
            }

            if (0 <= sym && sym < 256) {
                context->keyboard[sym] = true;
            }
        }
        break;

        case SDL_MOUSEMOTION: {
            context->mouse.x = event.motion.x;
            context->mouse.y = event.motion.y;
        }
        break;

        case SDL_MOUSEBUTTONDOWN: {
            context->mouse_pressed = true;
            context->mouse.x = event.motion.x;
            context->mouse.y = event.motion.y;
            context->drag_mouse = context->mouse;
        }
        break;

        case SDL_MOUSEBUTTONUP: {
            context->mouse_pressed = false;
            context->mouse.x = event.motion.x;
            context->mouse.y = event.motion.y;
            context->mouse_clicked = v2_dist(context->drag_mouse, context->mouse) < CLICK_THRESHOLD_DISTANCE;
        }
        break;

        case SDL_MOUSEWHEEL: {
            context->mouse_wheel_y += event.wheel.y;
        }
        break;
        }
    }

    if (!context->pause) {
        if (context->next_gen_timeout >= DELTA_TIME_SEC) {
            context->next_gen_cooldown -= DELTA_TIME_SEC;

            if (context->next_gen_cooldown <= 0.0) {
                context->next_gen_cooldown = context->next_gen_timeout;
                context->next_gen_count += 1;
            }
        } else {
            context->next_gen_count += (size_t) floorf(DELTA_TIME_SEC / context->next_gen_timeout);
        }
    }

    return context->quit;
}

void core_begin_rendering(Core *context)
{
    scc(SDL_SetRenderDrawColor(
            context->renderer,
            RGBA32_UNPACK(BACKGROUND_COLOR)));
    scc(SDL_RenderClear(context->renderer));
}

#define PAUSE_PADDING 50.0f
#define PAUSE_BAR_WIDTH 20.0f
#define PAUSE_BAR_HEIGHT 100.0f
#define PAUSE_BAR_GAP 20.0f
#define PAUSE_BAR_COLOR RED_COLOR
#define PAUSE_WIDTH (2.0f * PAUSE_BAR_WIDTH + PAUSE_BAR_GAP)
#define PAUSE_HEIGHT PAUSE_BAR_HEIGHT

void core_draw_pause_symbol(Core *context, float x, float y)
{
    Uint32 color = PAUSE_BAR_COLOR;

    if (x <= context->mouse.x &&
            context->mouse.x <= x + PAUSE_WIDTH &&
            y <= context->mouse.y &&
            context->mouse.y <= y + PAUSE_HEIGHT) {
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

size_t core_next_gen_count(Core *context)
{
    size_t result = context->next_gen_count;
    context->next_gen_count = 0;
    return result;
}

int mod(int a, int b)
{
    return (a % b + b) % b;
}

#endif  // CORE_H_
