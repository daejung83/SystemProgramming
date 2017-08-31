/**
 * Machine Problem 1
 * CS 241 - Spring 2016
 */

#include "document.h"
#include <stdio.h>
// test your document here
int main() { 
  //Document* doc = Document_create();
  Document* doc = Document_create_from_file("test.txt");

  Document_write_to_file(doc, "output.txt");
  
  //size_t size = Document_size(doc);
  //printf("size: %lu\n", size);
  
  for(size_t i = 0; i < Document_size(doc); i++){
    printf("%s\n", Document_get_line(doc,i));
  }

  Document_destroy(doc);
  
  Document* doc2 = Document_create();
  Document_insert_line(doc2, 0, NULL);
  Document_write_to_file(doc, "output.txt");

  return 0; 
}
