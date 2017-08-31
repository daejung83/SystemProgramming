OBJS_DIR = .objs

# define all of student executables
EXE1=vector_test
EXE2=document_test
EXES_STUDENT=$(EXE1) $(EXE2) 
EXES_PROVIDED=$(EXE1)-reference $(EXE2)-reference

# list object file dependencies for each
OBJS_PART1=vector.o vector_test.o
OBJS_PART2=document.o vector.o document_test.o

# don't include the vector or document in the objects for the -reference
# versions
OBJS_PART1a=vector_test.o
OBJS_PART2a=document_test.o

# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
CFLAGS_DEBUG   = -O0 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE
CFLAGS_RELEASE = -O2 $(WARNINGS) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# set up linker
LD = clang
LDFLAGS =
LDFLAGS-reference = -Llibs/ -lprovided

.PHONY: all
all: release

# build types
.PHONY: release
.PHONY: debug

release: $(EXES_STUDENT) $(EXES_PROVIDED)
debug:   clean $(EXES_STUDENT:%=%-debug) $(EXES_PROVIDED)

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
$(EXE1)-debug: $(OBJS_PART1:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE1): $(OBJS_PART1:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE1)-reference: $(OBJS_PART1a:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS-reference) -o $@

$(EXE2)-debug: $(OBJS_PART2:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE2): $(OBJS_PART2:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE2)-reference: $(OBJS_PART2a:%.o=$(OBJS_DIR)/%-release.o)
	$(LD) $^ $(LDFLAGS-reference) -o $@

.PHONY: clean
clean:
	rm -rf .objs $(EXES_STUDENT) $(EXES_STUDENT:%=%-debug)
