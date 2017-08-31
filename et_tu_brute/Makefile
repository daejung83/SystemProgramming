OBJS_DIR = .objs

# define all of student executables
EXE_BRUTUS=brutus
EXES_STUDENT=$(EXE_BRUTUS)

# list object file dependencies for each
OBJS_BRUTUS=brutus.o

# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
INCLUDES=-I./includes/
CFLAGS_DEBUG   = $(INCLUDES) -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = $(INCLUDES) -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = clang
LDFLAGS = -Llibs/ -lprovided

# the string in grep must appear in the hostname, otherwise the Makefile will
# not allow the assignment to compile
IS_VM=$(shell hostname | grep "sp16-cs241")

ifeq ($(IS_VM),)
$(error This assignment must be compiled on the CS241 VMs)
endif

.PHONY: all
all: release

# build types
.PHONY: release
.PHONY: debug

release: $(EXES_STUDENT)
debug:   clean $(EXES_STUDENT:%=%-debug)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# exes
# you will need a pair of exe and exe-debug targets for each exe
$(EXE_BRUTUS)-debug: $(OBJS_BRUTUS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_BRUTUS): $(OBJS_BRUTUS:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf .objs $(EXES_STUDENT) $(EXES_STUDENT:%=%-debug)
