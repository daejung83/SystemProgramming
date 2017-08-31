CC = gcc
INC = -I.
WARNINGS = -Wall -Werror -Wno-error=unused-parameter
FLAGS    = -O0 $(WARNINGS) $(INC) -g -std=c99 -D_GNU_SOURCE -DDEBUG
LINKOPTS = -pthread

all: wearable_sim wearable_server

wearable_sim: wearable.o queue.o wearable_sim.c
	$(CC) $^ $(FLAGS) $(LINKOPTS) -o $@

wearable.o: wearable.c wearable.h
	$(CC) -o $@ $< -c $(FLAGS) $(LINKOPTS)

queue.o: queue.c queue.h
	$(CC) -o $@ $< -c $(FLAGS)

wearable_server: wearable_server.c queue.o
	$(CC) $^ $(FLAGS) $(LINKOPTS) -o $@

.PHONY : clean
clean:
	-rm -f *.o *.so *.rp wearable_sim wearable_server
