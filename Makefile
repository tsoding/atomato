GL_PKGS=glfw3 glew

CFLAGS=-Wall -Wextra

all: rule110 viz

rule110: rule110.c
	$(CC) $(CFLAGS) -o rule110 rule110.c

viz: viz.c
	$(CC) $(CFLAGS) `pkg-config --cflags $(GL_PKGS)` -o viz viz.c `pkg-config --libs $(GL_PKGS)`

