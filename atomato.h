#ifndef ATOMATO_H_
#define ATOMATO_H_

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
#define SCREEN_HEIGHT 768

#define BACKGROUND_COLOR 0x181818ff
#define HEX_COLOR_UNPACK(color) \
  ((color >> (8 * 3)) & 0xFF),  \
  ((color >> (8 * 2)) & 0xFF),  \
  ((color >> (8 * 1)) & 0xFF),  \
  ((color >> (8 * 0)) & 0xFF)

#define ROWS 100
#define COLS 100
#define CELL_WIDTH ((float) SCREEN_WIDTH / (float) COLS)
#define CELL_HEIGHT ((float) SCREEN_HEIGHT / (float) ROWS)

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

#endif  // ATOMATO_H_
