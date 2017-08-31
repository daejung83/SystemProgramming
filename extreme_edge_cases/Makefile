CC = gcc
INC = -I.
FLAGS = -Wall -Wextra -Wno-unused -Wno-sign-compare -std=c99

all: camelCaser

camelCaser: camelCaser_tests.o camelCaser.o camelCaser_main.o
	$(CC) $^ -o $@

camelCaser_tests.o: camelCaser_tests.c camelCaser.h camelCaser_tests.h
	$(CC) -c $(INC) $(FLAGS) $< -o $@

camelCaser.o: camelCaser.c
	$(CC) -c $(INC) $(FLAGS) $< -o $@

camelCaser_main.o: camelCaser_main.c camelCaser_tests.h camelCaser.h
	$(CC) -c $(INC) $(FLAGS) $< -o $@

clean:
	rm -f *.o camelCaser
