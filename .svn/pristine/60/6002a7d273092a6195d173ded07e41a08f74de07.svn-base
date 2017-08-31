/**
 * Machine Problem 1
 * CS 241 - Spring 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "document.h"
#include "vector.h"

Document *Document_create() {
  Document *document = (Document *)malloc(sizeof(Document));
  document->vector = Vector_create();
  assert(document);
  return document;
}

void Document_write_to_file(Document *document, const char *filename) {
  assert(document);
  assert(filename);
  // see the comment in the header file for a description of how to do this!
  // your code here!
}

Document *Document_create_from_file(char *filename) {
  assert(filename);
  // this function will read a file which is created by Document_write_to_file
  // your code here!
  return NULL;
}

void Document_destroy(Document *document) {
  assert(document);
  Vector_destroy(document->vector);
  free(document);
}

size_t Document_size(Document *document) {
  assert(document);
  return Vector_size(document->vector);
}

void Document_set_line(Document *document, size_t index, const char *str) {
  assert(document);
  Vector_set(document->vector, index, str);
}

const char *Document_get_line(Document *document, size_t index) {
  assert(document);
  return Vector_get(document->vector, index);
}

void Document_insert_line(Document *document, size_t index, const char *str) {
  assert(document);
  Vector_insert(document->vector, index, str);
}

void Document_delete_line(Document *document, size_t index) {
  assert(document);
  Vector_delete(document->vector, index);
}
