OBJS_DIR = .objs

# the really challenging part in this makefile is tying all the components
# together

# assume all mappers have the same deps, this makes it way easier to build
# mappername.o is added to each of the deps when the target is invoked
MAPPERS_SRCS=$(wildcard mappers/*.c)
MAPPERS=$(MAPPERS_SRCS:mappers/%.c=mapper_%)
MAPPERS_DEPS=core/mapper.o

# same deal for reducers
REDUCERS_SRCS=$(wildcard reducers/*.c)
REDUCERS=$(REDUCERS_SRCS:reducers/%.c=reducer_%)
REDUCERS_DEPS=core/reducer.o core/libds.o core/common.o

# pi is a little different
# I've just hardcoded those targets in this file

# all of the connector tools
# again, I'm asserting that all of these have the same deps
TOOLS=mr0 mr1 mr2 shuffler splitter
TOOLS_DEPS=core/common.o

# set up compiler
CC = clang
WARNINGS = -Wall -Wextra -Werror -Wno-error=unused-parameter
INC=-Icore/
CFLAGS_DEBUG   = -O0 $(WARNINGS) $(INC) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE -DDEBUG
CFLAGS_RELEASE = -O2 $(WARNINGS) $(INC) -g -std=c99 -c -MMD -MP -D_GNU_SOURCE

# pi stuff needs a c++ compiler
# we never build pi in debug mode
CXX = clang++
CXXFLAGS=-O3 -Wall -Icore/

# set up linker
LD = clang
LDFLAGS =

# the string in grep must appear in the hostname, otherwise the Makefile will
# not allow the assignment to compile
IS_VM=$(shell hostname | grep "sp16-cs241")

ifeq ($(IS_VM),)
$(error This assignment must be compiled on the CS241 VMs)
endif

.PHONY: all
all: release

# build types
# run clean before building debug so that all of the release executables
# disappear
.PHONY: debug
.PHONY: release

release: mappers-release reducers-release pi tools-release
debug:   clean mappers-debug reducers-debug pi tools-debug

.PHONY: mappers-relase
mappers-release: $(MAPPERS)

.PHONY: mappers-debug
mappers-debug : $(MAPPERS:%=%-debug)

.PHONY: reducers-relase
reducers-release: $(REDUCERS)

.PHONY: reducers-debug
reducers-debug : $(REDUCERS:%=%-debug)

.PHONY: tools-release
tools-release: $(TOOLS)

.PHONY: tools-debug
tools-debug: $(TOOLS:%=%-debug)

.PHONY: pi
pi: mapper_pi reducer_pi

.PHONY: data
data: data/alice.txt data/dracula.txt

data/alice.txt:
		curl "http://www.gutenberg.org/files/11/11.txt" > data/alice.txt

data/dracula.txt:
		curl "http://www.gutenberg.org/cache/epub/345/pg345.txt" > data/dracula.txt

# include dependencies
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
-include $(shell find $(ROOT_DIR)/$(OBJS_DIR) -name '*.d')

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

# patterns to create objects
# keep the debug and release postfix for object files so that we can always
# separate them correctly
$(OBJS_DIR)/%-debug.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_DEBUG) $< -o $@

$(OBJS_DIR)/%-release.o: %.c | $(OBJS_DIR)
	@mkdir -p $(basename $@)
	$(CC) $(CFLAGS_RELEASE) $< -o $@

# executables
$(MAPPERS): mapper_% : $(MAPPERS_DEPS:%.o=$(OBJS_DIR)/%-release.o) $(OBJS_DIR)/mappers/%-release.o
	$(LD) $^ $(LDFLAGS) -o $@

$(MAPPERS:%=%-debug): mapper_%-debug : $(MAPPERS_DEPS:%.o=$(OBJS_DIR)/%-debug.o) $(OBJS_DIR)/mappers/%-debug.o
	$(LD) $^ $(LDFLAGS) -o $@

$(REDUCERS): reducer_% : $(REDUCERS_DEPS:%.o=$(OBJS_DIR)/%-release.o) $(OBJS_DIR)/reducers/%-release.o
	$(LD) $^ $(LDFLAGS) -o $@

$(REDUCERS:%=%-debug): reducer_%-debug : $(REDUCERS_DEPS:%.o=$(OBJS_DIR)/%-debug.o) $(OBJS_DIR)/reducers/%-debug.o
	$(LD) $^ $(LDFLAGS) -o $@

$(TOOLS): % : $(TOOLS_DEPS:%.o=$(OBJS_DIR)/%-release.o) $(OBJS_DIR)/%-release.o
	$(LD) $^ $(LDFLAGS) -o $@

$(TOOLS:%=%-debug): %-debug : $(TOOLS_DEPS:%.o=$(OBJS_DIR)/%-debug.o) $(OBJS_DIR)/%-debug.o
	$(LD) $^ $(LDFLAGS) -o $@

# pi stuff
mapper_pi: $(OBJS_DIR)/core/mapper-release.o pi/mapper_pi.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

reducer_pi: $(OBJS_DIR)/core/reducer-release.o $(OBJS_DIR)/core/libds-release.o pi/reducer_pi.cpp $(OBJS_DIR)/core/common-release.o
	$(CXX) $(CXXFLAGS) $^ -o $@

## TESTING
COMMON_TESTER=unit_tests/test_common
COMMON_TESTER_DEPS=core/common.o unit_tests/test_common.o

MAPPER_TESTER=unit_tests/test_mapper
MAPPER_TESTER_DEPS=core/mapper.o unit_tests/test_mapper.o

REDUCER_TESTER=unit_tests/test_reducer
REDUCER_TESTER_DEPS=core/reducer.o unit_tests/test_reducer.o core/libds.o core/common.o

MAPPER_ASCIICOUNT_TESTER=unit_tests/mappers/asciicount_test
MAPPER_ASCIICOUNT_TESTER_DEPS=core/mapper.o $(MAPPER_ASCIICOUNT_TESTER).o core/common.o


MAPPER_LETTERCOUNT_TESTER=unit_tests/mappers/lettercount_test
MAPPER_LETTERCOUNT_TESTER_DEPS=core/mapper.o $(MAPPER_LETTERCOUNT_TESTER).o core/common.o

MAPPER_WORDCOUNT_TESTER=unit_tests/mappers/wordcount_test
MAPPER_WORDCOUNT_TESTER_DEPS=core/mapper.o $(MAPPER_WORDCOUNT_TESTER).o core/common.o

MAPPER_WORDLENGTHS_TESTER=unit_tests/mappers/wordlengths_test
MAPPER_WORDLENGTHS_TESTER_DEPS=core/mapper.o $(MAPPER_WORDLENGTHS_TESTER).o core/common.o

TESTERS=$(COMMON_TESTER) $(MAPPER_TESTER) $(REDUCER_TESTER)
TESTERS+=$(MAPPER_ASCIICOUNT_TESTER)
TESTERS+=$(MAPPER_LETTERCOUNT_TESTER)
TESTERS+=$(MAPPER_WORDCOUNT_TESTER)
TESTERS+=$(MAPPER_WORDLENGTHS_TESTER)

$(COMMON_TESTER): $(COMMON_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@

$(MAPPER_TESTER): $(MAPPER_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@

$(REDUCER_TESTER): $(REDUCER_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@

$(MAPPER_ASCIICOUNT_TESTER): $(MAPPER_ASCIICOUNT_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	#@echo -e '\nStart ASCIICOUNT TEST\n'
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@
	#@echo -e '\nTest ASCIICOUNT Done\n\n'


$(MAPPER_LETTERCOUNT_TESTER): $(MAPPER_LETTERCOUNT_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	#@echo -e '\nStart LETTERCOUNT TEST\n'
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@
	#@echo -e '\nTest LETTERCOUNT Done\n\n'

$(MAPPER_WORDCOUNT_TESTER): $(MAPPER_WORDCOUNT_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	#@echo -e '\nStart WORDCOUNT TEST\n'
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@
	#@echo -e '\nTest WORDCOUNT Done\n\n'

$(MAPPER_WORDLENGTHS_TESTER): $(MAPPER_WORDLENGTHS_TESTER_DEPS:%.o=$(OBJS_DIR)/%-debug.o)
	$(LD) $^ $(LDFLAGS) -o $@
	#@echo -e '\nStart WORDLENGTHS TEST\n'
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=all $@
	#@echo -e '\nTest WORDLENGTHS Done\n\n'

.PHONY: test
test: $(TESTERS)

.PHONY: clean
clean:
	-rm -rf $(MAPPERS) $(MAPPERS:%=%-debug)
	-rm -rf $(REDUCERS) $(REDUCERS:%=%-debug)
	-rm -rf mapper_pi reducer_pi
	-rm -rf $(TOOLS) $(TOOLS:%=%-debug)
	-rm -rf $(TESTERS)
	-rm -rf .objs $(EXES_STUDENT)
