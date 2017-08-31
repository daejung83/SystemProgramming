CC = gcc
INC = -I.
FLAGS = -Wall -Wextra -Wno-unused -Wno-sign-compare -std=c99

all: camelCaser

camelCaser: camelCaser.o camelCaser_main.o
	$(CC) $^ -o $@

camelCaser_main.o: camelCaser_main.c camelCaser.h
	$(CC) -c $(INC) $(FLAGS) $< -o $@

clean:
	rm -f camelCaser_main.o camelCaser
