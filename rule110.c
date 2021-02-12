#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./atomato.h"

typedef enum {
    O = 0,
    I = 1,
} Cell;

Uint32 cell_color[2] = {
    [O] = 0x00000000,
    [I] = 0xFFAABBFF,
};

#define PATTERN(A, B, C) ((A << 2) | (B << 1) | C)

Cell patterns[1 << 3] = {
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
    Cell cells[COLS];
} Row;

void render_row(SDL_Renderer *renderer, Row row, int y)
{
    for (int i = 0; i < COLS; ++i) {
        const SDL_Rect rect = {
            .x = (int) floorf(i * CELL_WIDTH),
            .y = y,
            .w = (int) floorf(CELL_WIDTH),
            .h = (int) floorf(CELL_HEIGHT)
        };

        scc(SDL_SetRenderDrawColor(
                renderer,
                HEX_COLOR_UNPACK(cell_color[row.cells[i]])));

        scc(SDL_RenderFillRect(renderer, &rect));
    }
}

Row next_row(Row prev)
{
    Row next = {0};

    // N = 5
    // 01234
    // *****
    //  ^

    for (int i = 1; i < COLS - 1; ++i) {
        const int index = PATTERN(prev.cells[i - 1],
                                  prev.cells[i],
                                  prev.cells[i + 1]);
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

int main(void)
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window * const window = atomato_create_window();
    SDL_Renderer * const renderer = atomato_create_renderer(window);

    bool quit = false;

    Row rows[ROWS];

    _Static_assert(ROWS > 0, "We need to have at least 1 row");
    rows[0] = random_row();

    for (int i = 1; i < ROWS; ++i) {
        rows[i] = next_row(rows[i - 1]);
    }

    while (!quit) {
        SDL_Event event;
        while (!SDL_PollEvent(&event)) {
            switch(event.type) {
            case SDL_QUIT: {
                quit = true;
            }
            break;
            }
        }

        scc(SDL_SetRenderDrawColor(
                renderer,
                HEX_COLOR_UNPACK(BACKGROUND_COLOR)));
        scc(SDL_RenderClear(renderer));

        for (int i = 0; i < ROWS; ++i) {
            render_row(renderer, rows[i], i * CELL_HEIGHT);
        }

        SDL_RenderPresent(renderer);

        SDL_Delay(10);
    }

    SDL_Quit();

    return 0;
}
