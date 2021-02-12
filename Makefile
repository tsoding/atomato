CFLAGS=-Wall -Wextra

all: rule110

rule110: rule110.c
	$(CC) $(CFLAGS) -o rule110 rule110.c
