OBJS_DIR = .objs

# define all of student executables
EXE_MEMTEST=memtest
EXE_MEMTESTR=memtest-reference
EXES_STUDENT=$(EXE_MEMTEST)
EXES_PROVIDED=$(EXE_MEMTESTR)

# list object file dependencies for each
OBJS_MEMTEST=test.o mini_valgrind.o
OBJS_MEMTESTR=test.o mini_valgrind-reference.o

# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
CFLAGS_DEBUG   = -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = -O2 $(WARNINGS)    -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = clang
LDFLAGS =

.PHONY: all
all: release

# build types
.PHONY: release
.PHONY: debug

release: $(EXES_STUDENT) $(EXES_PROVIDED)
debug:   clean $(EXES_STUDENT:%=%-debug)

# include dependencies
-include $(OBJS_DIR)/*.d

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@cp mini_valgrind-reference-release.o $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# exes
# you will need a pair of exe and exe-debug targets for each exe
$(EXE_MEMTEST)-debug: $(OBJS_MEMTEST:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_MEMTEST): $(OBJS_MEMTEST:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE_MEMTESTR): $(OBJS_MEMTESTR:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	rm -rf .objs $(EXES_STUDENT) $(EXES_STUDENT:%=%-debug) $(EXES_PROVIDED)
