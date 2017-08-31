#
# Utilities Unleashed
# CS 241 Fall 2015
#

CC = clang
INC = -I.
FLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -g -std=c99

all: time env

format.o: format.c format.h
	$(CC) -c $(FLAGS) $(INC) $< -o $@

time: time.o format.o
	$(CC) $^ -o $@

time.o: time.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

env: env.o format.o
	$(CC) $^ -o $@

env.o: env.c
	$(CC) -c $(FLAGS) $(INC) $< -o $@

.PHONY : clean
clean:
	-rm -r -f *.o time env
