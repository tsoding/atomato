// https://en.wikipedia.org/wiki/Seeds_(cellular_automaton)

#include "./atomato.h"

typedef enum {
    OFF = 0,
    ON = 1,
} Cell;

Uint32 cell_color[2] = {
    [OFF] = BACKGROUND_COLOR,
    [ON] = FOREGROUND_COLOR
};

typedef struct {
    Cell cells[ROWS][COLS];
} Board;

int board_nbors(const Board *board, int row0, int col0)
{
    int result = 0;
    for (int drow = -1; drow <= 1; ++drow) {
        for (int dcol = -1; dcol <= 1; ++dcol) {
            if (drow != 0 || dcol != 0) {
                const int row = mod(row0 + drow, ROWS);
                const int col = mod(col0 + dcol, COLS);

                if (board->cells[row][col] == ON) {
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
            if (prev->cells[row][col] == OFF && nbors == 2) {
                next->cells[row][col] = ON;
            } else {
                next->cells[row][col] = OFF;
            }
        }
    }
}

void render_board(const Board *board)
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            atomato_fill_rect(
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

Board board[2] = {0};
int fg = 0;

void seeds_event_callback(const SDL_Event *event)
{
    switch (event->type) {
    case SDL_MOUSEBUTTONDOWN: {
        const int col = (int) floorf(event->button.x / CELL_WIDTH);
        const int row = (int) floorf(event->button.y / CELL_HEIGHT);

        board[fg].cells[row][col] = 1 - board[fg].cells[row][col];
    }
    break;

    case SDL_KEYDOWN: {
        if (event->key.keysym.sym == SDLK_r) {
            memset(board[fg].cells, 0, sizeof(Cell) * ROWS * COLS);
        }
    }
    break;
    }
}

int main(void)
{
    atomato_begin();

    // random_board(&board[fg]);

    while (!atomato_time_to_quit()) {
        atomato_poll_events(seeds_event_callback);

        if (atomato_is_next_gen()) {
            int bg = 1 - fg;
            next_board(&board[fg], &board[bg]);
            fg = bg;
        }

        atomato_begin_rendering();
        {
            render_board(&board[fg]);
        }
        atomato_end_rendering();
    }

    atomato_end();
    return 0;
}
