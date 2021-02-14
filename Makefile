PKGS=sdl2

CFLAGS=-Wall -Wextra -Wno-missing-braces -std=c11 -pedantic -O3 `pkg-config --cflags $(PKGS)`
LIBS=`pkg-config --libs $(PKGS)` -lm

STBS=stb_image.o stb_image_write.o
FRAMEWORKS=core.h square.h life.h

all: rule110 gol seeds bb wireworld

%: %.c $(STBS) $(FRAMEWORKS)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS) $(STBS)

stb_image.o: stb_image.h
	$(CC) -O3 -DSTB_IMAGE_IMPLEMENTATION -x c -c -o stb_image.o stb_image.h

stb_image_write.o: stb_image_write.h
	$(CC) -O3 -DSTB_IMAGE_WRITE_IMPLEMENTATION -x c -c -o stb_image_write.o stb_image_write.h
