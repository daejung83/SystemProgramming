SHELL=/bin/bash
CC = clang
INC = -I.
FLAGS = -Ofast -Werror -Wall -Wextra -Wno-unused-parameter -std=c99
DEBUG_FLAGS = -g -Werror -Wall -Wextra -Wno-unused-parameter -std=c99
LINKOPTS = -pthread

all: barrier_test poisson_test

reference: barrier_test-reference poisson_test-reference

debug: barrier_test-debug poisson_test-debug

poisson_test: poisson_test.c poisson.o bmp.o barrier.o
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -lm -lrt $^ -o $@

poisson.o: poisson.c poisson.h bmp.o barrier.o
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -D _GNU_SOURCE -c $<

bmp.o: bmp.c bmp.h
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -D _GNU_SOURCE -c $<

barrier_test: barrier_test.c barrier.o
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -D _GNU_SOURCE $^ -o $@

barrier.o: barrier.c barrier.h
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -c $<

poisson_test-debug: poisson_test.c poisson-debug.o bmp-debug.o barrier-debug.o
	$(CC) $(INC) $(DEBUG_FLAGS) $(LINKOPTS) -lm -lrt $^ -o $@

poisson-debug.o: poisson.c poisson.h bmp-debug.o barrier-debug.o
	$(CC) $(INC) $(DEBUG_FLAGS) $(LINKOPTS) -D _GNU_SOURCE -c $< -o $@

bmp-debug.o: bmp.c bmp.h
	$(CC) $(INC) $(DEBUG_FLAGS) $(LINKOPTS) -D _GNU_SOURCE -c $< -o $@

barrier_test-debug: barrier_test.c barrier-debug.o
	$(CC) $(INC) $(DEBUG_FLAGS) $(LINKOPTS) -D _GNU_SOURCE $^ -o $@

barrier-debug.o: barrier.c barrier.h
	$(CC) $(INC) $(DEBUG_FLAGS) $(LINKOPTS) -c $< -o $@

poisson_test-reference: poisson_test.c bmp.o
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -L./libs $^ -Wl,-Bstatic -lpoisson -lbarrier -Wl,-Bdynamic -lm -lrt -o $@

barrier_test-reference: barrier_test.c 
	$(CC) $(INC) $(FLAGS) $(LINKOPTS) -D _GNU_SOURCE -L./libs $^ -Wl,-Bstatic -lbarrier -Wl,-Bdynamic -o $@

clean:
	-rm -r -f *.o *.bmp barrier_test poisson_test barrier_test-* poisson_test-*
