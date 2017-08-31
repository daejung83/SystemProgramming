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

#include "common.h"

void test_spl() {
  char *key = NULL;
  char *value = NULL;

  char *line = malloc(1000);
  memset(line, '\0', sizeof(char) * 1000);
  strcpy(line, "a: b\n");

  assert(split_key_value(line, &key, &value));
  assert(0 == strcmp(key, "a"));
  assert(0 == strcmp(value, "b"));

  memset(line, '\0', sizeof(char) * 1000);
  strcpy(line, "a b\n");
  assert(!split_key_value(line, &key, &value));

  memset(line, '\0', sizeof(char) * 1000);
  strcpy(line, "a: b");
  assert(!split_key_value(line, &key, &value));

  // make sure it doesn't eat spaces
  memset(line, '\0', sizeof(char) * 1000);
  strcpy(line, "a:   b\n");
  assert(split_key_value(line, &key, &value));
  assert(0 == strcmp(key, "a"));
  assert(0 == strcmp(value, "  b"));

  free(line);
}

int fd_is_valid(int fd) { return fcntl(fd, F_GETFD) != -1 || errno != EBADF; }

int main() {
  // create a bunch of fds, add most of them to the descriptors thing
  for (int i = 3; i < 10; i++) {
    dup2(0, i);
  }

  for (int i = 6; i < 10; i++) {
    descriptors_add(i);
  }

  // all of the fds should be open at this point
  for (int i = 3; i < 10; i++) {
    assert(fd_is_valid(i));
  }

  descriptors_closeall();

  for (int i = 3; i < 6; i++) {
    assert(fd_is_valid(i));
  }

  descriptors_destroy();

  test_spl();
  printf("common test passed\n");

  return 0;
}
