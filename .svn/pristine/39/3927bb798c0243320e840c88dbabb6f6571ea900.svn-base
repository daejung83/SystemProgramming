#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mini_valgrind.h"
int main() {
  // your tests here using malloc and free
  printf("start of test\n");
  char* hello_world = malloc(255);
  printf("before strcpy\n");
  strcpy(hello_world, "hi");
  printf("hi %s\n", hello_world);
  free(hello_world);
  
  char* test1 = malloc(123);
  char* test2 = malloc(234);
  //test1 = realloc(test1, 200);
  char* test3 = malloc(532);
  strcpy(test1, "test1");
  strcpy(test2, "test2");
  strcpy(test3, "test3");
  printf("%s %s %s\n", test1, test2, test3);
  free(test1);
  free(test2);
  free(test3);
  free(test3);
  char* test4 = malloc(0);
  free(test4);
  free(test3);
  free(test3);
  printf("total_usage: %zu\n", total_usage);
  printf("total_free: %zu\n", total_free);
  printf("bad_frees: %zu\n", bad_frees);

  //free(hello_world); 
  // Do NOT modify this line
  atexit(print_report);
  return 0;
}
