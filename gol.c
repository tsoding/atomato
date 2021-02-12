#include <assert.h>
#include <stdbool.h>
#include "./atomato.h"

typedef enum {
    DED = 0,
    ALIVE = 1,
} Cell;

Uint32 cell_color[2] = {
    [DED] = 0x00000000,
    [ALIVE] = 0xFFAABBFF
};

typedef struct {
    Cell cells[ROWS][COLS];
} Board;

void render_board(SDL_Renderer *renderer, const Board *board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            atomato_fill_rect(
                renderer,
                col * CELL_WIDTH,
                row * CELL_HEIGHT,
                CELL_WIDTH,
                CELL_HEIGHT,
                cell_color[board->cells[row][col]]);
        }
    }
}

void random_board(Board *board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            board->cells[row][col] = rand() % 2;
        }
    }
}

int board_nbors(const Board *board, int row0, int col0)
{
    int result = 0;
    for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
            if (drow != 0 || dcol != 0) {
                const int row = mod(row0 + drow, ROWS);
                const int col = mod(col0 + dcol, COLS);
                if (board->cells[row][col] == ALIVE) {
                    result += 1;
                }
            }
        }
    }
    return result;
}

void next_board(const Board *prev, Board *next)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            const int nbors = board_nbors(prev, row, col);

            switch (prev->cells[row][col]) {
            case DED: {
                next->cells[row][col] = nbors == 3 ? ALIVE : DED;
            }
            break;
            case ALIVE: {
                next->cells[row][col] = nbors == 2 || nbors == 3 ? ALIVE : DED;
            }
            break;
            default:
                assert(false && "next_board: unreachable");
            }
        }
    }
}

void put_glider_at(Board *board, int row0, int col0)
{
    Cell glider[3][3] = {
        {DED, ALIVE, DED},
        {DED, DED,   ALIVE},
        {ALIVE, ALIVE, ALIVE},
    };

    for (int drow = 0; drow < 3; ++drow) {
        for (int dcol = 0; dcol < 3; ++dcol) {
            const int row = mod(row0 + drow, ROWS);
            const int col = mod(col0 + dcol, COLS);
            board->cells[row][col] = glider[drow][dcol];
        }
    }
}

Board board[2] = {0};
int fg = 0;

int main()
{
    scc(SDL_Init(SDL_INIT_VIDEO));

    SDL_Window *window = atomato_create_window();
    SDL_Renderer *renderer = atomato_create_renderer(window);

    bool quit = false;

    // random_board(&board[fg]);

    put_glider_at(&board[fg], 0, 0);
    put_glider_at(&board[fg], 4, 4);

    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
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

        render_board(renderer, &board[fg]);

        const int bg = 1 - fg;
        next_board(&board[fg], &board[bg]);
        fg = bg;

        SDL_RenderPresent(renderer);

        SDL_Delay(150);
    }

    SDL_Quit();
    return 0;
}
