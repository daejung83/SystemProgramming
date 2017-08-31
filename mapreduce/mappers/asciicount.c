/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "mapper.h"

void mapper(const char *data, FILE *output) {
  while (*data) {
    int c = *data++;
    if (isalpha(c)) {
      c = tolower(c);
      fprintf(output, "%c: 1\n", c);
    }
  }
}

MAKE_MAPPER_MAIN(mapper)
