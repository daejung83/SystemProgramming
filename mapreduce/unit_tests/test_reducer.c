/**
 * MapReduce
 * CS 241 - Spring 2016
 */

// this is a pretty hacky tester :)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "reducer.h"
#include "common.h"

static int seen[2] = { 0, 0 };
void ive_seen(const char *a) {
  printf("I've seen: %s\n", a);

  if (strcmp(a, "carman"))
    seen[0] = 1;

  if (strcmp(a, "cartman"))
    seen[1] = 1;
}

const char *seen_tester(const char *a, const char *b) {
  char *trash = malloc(1);
  *trash = '\0';

  printf("called!\n");
  ive_seen(a);
  ive_seen(b);

  return trash;
}

const char *output_tester(const char *a, const char *b) {
  printf("output_tester called with values %s and %s\n", a, b);
  char *res = malloc(4);
  strcpy(res, "200");
  return res;
}

int main() {

  // make sure the buffer is the right size
  // reducer depends on EOF and having every line terminated with a \n
  char *str = "a: carman\na: cartman\ncats-and-dogs: 3\n";

  char *buffer = alloca(strlen(str) + 1);
  memset(buffer, '\0', sizeof(char) * strlen(str) + 1);
  strcpy(buffer, str);

  FILE *buffer_ptr = fmemopen(buffer, strlen(str), "r");
  assert(buffer_ptr);

  // test that we actually get the input we wanted
  run_reducer_on(buffer_ptr, stdout, seen_tester);

  for (size_t i = 0; i < 2; i++) {
    printf("checking for i=%lu\n", i);
    assert(seen[i] == 1);
  }

  printf("next test\n");
  fclose(buffer_ptr);

  // test the output
  str = "a: 100\nb: 200\na: 100\n";
  buffer = alloca(strlen(str) + 1);
  memset(buffer, '\0', sizeof(char) * strlen(str) + 1);
  strcpy(buffer, str);
  buffer_ptr = fmemopen(buffer, strlen(buffer), "r");

  char obuff[2000];
  memset(obuff, '\0', sizeof(char) * 2000);
  FILE *outptr = fmemopen(obuff, 2000, "w");

  run_reducer_on(buffer_ptr, outptr, output_tester);
  fclose(outptr);
  outptr = fmemopen(obuff, strlen(obuff), "r");

  char *line = NULL;
  size_t size = 0;
  while (getline(&line, &size, outptr) != -1) {
    char *key = NULL;
    char *value = NULL;
    assert(split_key_value(line, &key, &value));
    assert(0 == strcmp(value, "200"));
  }

  free(line);
  fclose(outptr);

  fclose(buffer_ptr);
  printf("reducer test passed\n");

  return 0;
}
