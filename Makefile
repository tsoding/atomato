PKGS=sdl2

CFLAGS=-Wall -Wextra -std=c11 -pedantic `pkg-config --cflags $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)` -lm

all: rule110 gol seeds

%: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
