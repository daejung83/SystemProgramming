/**
 * MapReduce
 * CS 241 - Spring 2016
 */

// wanna see the biggest hack in the book?
// look no further than this file!

// first we include mapper.h so that it won't be included by the files I'm
// about to include (double inclusion guards).

#include "mapper.h"

// now I'm going to change the MAKE_MAPPER_MAIN macro into a noop, to prevent
// this code from having multiple mains
#undef MAKE_MAPPER_MAIN
#define MAKE_MAPPER_MAIN(func)

// then I include the C file for the mapper function I want to test
// the MAKE_MAPPER_MAIN macro will not expand to anything interesting, we will
// just get a mapper function included.
// use the absolute path to prevent having to add a C file to the include path

#include "../../mappers/lettercount.c"

// now we have the mapper from asciicount included in our current "namespace,"
// but we haven't yet defined a main method

#include <assert.h>

#include "common.h"

#define SIZE 1000

int main() {
  char *no_words = "";

  char buffer[SIZE];
  memset(buffer, '\0', sizeof(char) * SIZE);
  FILE *obuff = fmemopen(buffer, SIZE, "w");

  mapper(no_words, obuff);
  fclose(obuff);

  assert(strlen(buffer) == 0);

  char *no_letter = "92813109-0-  -0-09-0./";
  obuff = fmemopen(buffer, SIZE, "w");
  mapper(no_letter, obuff);
  fclose(obuff);

  assert(strlen(buffer) == 0);

  char multiple_line[SIZE];
  memset(multiple_line, 0, sizeof(char) * SIZE);
  sprintf(multiple_line, "\n%s\n%s\n\n%s\n", "123aSS", "af20baba", "bb");

  obuff = fmemopen(buffer, SIZE, "w");
  mapper(multiple_line, obuff);
  fclose(obuff);

  // use size of strlen buffer to force it to send EOF
  obuff = fmemopen(buffer, strlen(buffer), "r");
  size_t line_counter = 0;

  char *line = NULL;
  size_t size = 0;

  while (getline(&line, &size, obuff) != -1) {
    char *key = NULL;
    char *value = NULL;
    assert(split_key_value(line, &key, &value));

    if (0 == strcmp("letters", key) && 0 == strcmp(value, "1"))
      line_counter++;
  }

  assert(line_counter == 11);

  free(line);
  fclose(obuff);

  return 0;
}
