// https://en.wikipedia.org/wiki/Rule_110

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define ROW_SIZE 64
#define ITERATIONS 100

typedef enum {
    O = 0,
    I = 1,
} Cell;

char cell_image[2] = {
    [O] = ' ',
    [I] = '*'
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
    Cell cells[ROW_SIZE];
} Row;

Row next_row(Row prev)
{
    Row next = {0};

    // N = 5
    // 01234
    // *****
    //  ^

    for (int i = 1; i < ROW_SIZE - 1; ++i) {
        const int index = PATTERN(prev.cells[i - 1],
                                  prev.cells[i],
                                  prev.cells[i + 1]);
        next.cells[i] = patterns[index];
    }

    return next;
}

void print_row(Row row)
{
    putc('|', stdout);
    for (int i = 0; i < ROW_SIZE; ++i) {
        putc(cell_image[row.cells[i]], stdout);
    }
    putc('|', stdout);
    putc('\n', stdout);
}

Row random_row(void)
{
    Row result = {0};

    for (int i = 0; i < ROW_SIZE; ++i) {
        result.cells[i] = rand() % 2;
    }

    return result;
}

int main()
{
    srand(time(0));
    Row row = random_row();

    for (int i = 0; i < ITERATIONS; ++i) {
        print_row(row);
        row = next_row(row);
    }

    return 0;
}
