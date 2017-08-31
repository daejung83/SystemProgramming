/**
 * Machine Problem 1
 * CS 241 - Spring 2016
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
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
  //FILE *fopen( const char * filename, const char * mode );  
  FILE *fp;
  fp = fopen(filename, "w");
  char* str = malloc(255); 
  for(size_t i = 0; i < Document_size(document); i++){
    //printf("does it seg\n");
    if(Document_get_line(document, i) != NULL)
      strcpy(str, Document_get_line(document, i));
    strcat(str, "\n");
    if(str != NULL){
      fputs("\n", fp);
    }else{
      strcpy(str, "\n");
      fputs(str,fp);
    }
  }
  free(str);
  //if(!feof == fclose(fp))
  //  printf("file closed with out error\n");
  //else
  //  printf("file closing failed\n");
}

Document *Document_create_from_file(char *filename) {
  assert(filename);
  // this function will read a file which is created by Document_write_to_file
  // your code here!i

  Document* doc = Document_create();
  FILE* fp;
  char* str = NULL;
  size_t len = 0;
  ssize_t read;
  fp = fopen(filename, "r");
  while((read = getline(&str, &len, fp)) != -1){
    str[strlen(str)-1] = 0;
    Document_insert_line(doc, Document_size(doc),str); 
  }
  fclose(fp);
  free(str);
  return doc;
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
