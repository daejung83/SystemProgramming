/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "mapper.h"

void mapper(const char *data, FILE *output) {
  while (*data) {
    int c = *data++;
    if (isalpha(c)) {
      fprintf(output, "letters: 1\n");
    }
  }
}

MAKE_MAPPER_MAIN(mapper)
