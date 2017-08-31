/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "mapper.h"

#define BUFFER_SIZE 2000

// test if the pointer makes it through
// don't worry about why it is called this
void deadbeef_test(const char __attribute__((unused)) * unused, FILE *fno) {
  assert(fno == stdout);
}

static int test_input_counter = 0;
void test_input(const char *dat, FILE __attribute__((unused)) * fno) {
  switch (test_input_counter) {
  case 0:
    assert(0 == strcmp(dat, "hello world"));
    break;

  case 1:
    assert(0 == strcmp(dat, "goodbye"));
    break;

  default:
    assert(0 && "the test is probably broken");
    break;
  }

  test_input_counter++;
}

int main() {
  char buffer[BUFFER_SIZE];
  memset(buffer, '\0', sizeof(char) * BUFFER_SIZE);

  strcpy(buffer, "hello world\ngoodbye");

  FILE *buffer_ptr = fmemopen(buffer, BUFFER_SIZE, "r");
  assert(buffer_ptr);

  // check that we pass the FILE* correctly
  run_mapper_on_fds(buffer_ptr, stdout, deadbeef_test);

  rewind(buffer_ptr);

  // check that we pass the buffer correctly
  run_mapper_on_fds(buffer_ptr, stdout, test_input);

  fclose(buffer_ptr);
  printf("mapper test passed\n");

  return 0;
}
