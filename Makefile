PKGS=sdl2

CFLAGS=-Wall -Wextra -Wno-missing-braces -std=c11 -pedantic -O3 `pkg-config --cflags $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)` -lm

all: rule110 gol seeds bb wireworld

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
