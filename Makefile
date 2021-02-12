GL_PKGS=sdl2

CFLAGS=-Wall -Wextra -std=c11 -pedantic

all: rule110

rule110: rule110.c
	$(CC) $(CFLAGS) `pkg-config --cflags $(GL_PKGS)` -o rule110 rule110.c `pkg-config --libs $(GL_PKGS)` -lm

