# see ../deploy.make.common for most of the instructions.

OBJS_DIR = .objs
OBJS_SCHEDULER_DIR = $(OBJS_DIR)/libscheduler
OBJS_PRIQUEUE_DIR = $(OBJS_DIR)/libpriqueue

# define simulator executables
EXE_SIMULATOR=simulator

# list objec file dependencies for each
OBJS_SIMULATOR=$(EXE_SIMULATOR).o
OBJS_SCHEDULER=libscheduler/libscheduler.o
OBJS_PRIQUEUE=libpriqueue/libpriqueue.o
OBJS_ALL=$(OBJS_SIMULATOR) $(OBJS_SCHEDULER) $(OBJS_PRIQUEUE)

# set up compiler
CC = gcc
WARNINGS = -Wall -Wextra -Wno-unused -Wno-unused-parameter
CFLAGS_DEBUG = -O0 $(WARNINGS) -g -std=c99 -c  -D_GNU_SOURCE
CFLAGS_RELEASE = -O2 $(WARNINGS) -std=c99 -c  -D_GNU_SOURCE
INC = -I.

# The string in grep must appear in the hostname, otherwise the Makefile will
# not allow the assignment to compile
IS_VM=$(shell hostname | grep "sp16-cs241")

ifeq ($(IS_VM),)
$(error This assignment must be compiled on the CS241 VMs)
endif

.PHONY: all
all: release

.PHONY: debug
.PHONY: release

release: $(EXE_SIMULATOR)
debug:   $(EXE_SIMULATOR:%=%-debug)

#include dependencies
-include $(OBJS_DIR)/*.d
$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR) $(OBJS_SCHEDULER_DIR) $(OBJS_PRIQUEUE_DIR)
# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $(INC) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
		$(CC) $(CFLAGS_RELEASE) $(INC) $< -o $@

# exes
# you will need exe and exe-debug
$(EXE_SIMULATOR): $(OBJS_ALL:%.o=$(OBJS_DIR)/%-release.o)
	$(CC) $^ -o $@

$(EXE_SIMULATOR)-debug: $(OBJS_ALL:%.o=$(OBJS_DIR)/%-debug.o)
	$(CC) $^ -o $@


#simulator: simulator.o #libscheduler/libscheduler.o #libpriqueue/libpriqueue.o
#	$(CC) $^ -o $@

#libscheduler/libscheduler.o: #libscheduler/libscheduler.c #libscheduler/libscheduler.h
#	$(CC) -c $(FLAGS) $(INC) $< -o $@

#libpriqueue/libpriqueue.o: #libpriqueue/libpriqueue.c #libpriqueue/libpriqueue.h
#	$(CC) -c $(FLAGS) $(INC) $< -o $@

#simulator.o: simulator.c #libscheduler/libscheduler.h
#	$(CC) -c $(FLAGS) $(INC) $< -o $@


.PHONY : clean
clean:
	rm -rf .objs $(EXES_SIMULATOR) $(EXES_SIMULATOR:%=%-debug)
