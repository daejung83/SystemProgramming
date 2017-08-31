/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2016
 */
#include <time.h>
#include "utils.h"
#include "tree.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses mmap to access the data.

  ./lookup2 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) { 
  
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start); 
  if(argc < 3){
    printf("\n\tlookup1 <data_file> <word> [<word> ...]\n\n");
    return 1;
  }
  int fp = open(argv[1], O_RDONLY);
  if(fp == -1){
    //printf("Cannot open file %s\n", argv[1]);
    openFail(argv[1]);
    return 2;
  }
  struct stat mystat;

  if(fstat(fp, &mystat) < 0){
    perror("fstat failed\n");
    close(fp);
    exit(1);
  }
  
  void* file_map = mmap(0, mystat.st_size, PROT_READ, MAP_PRIVATE, fp, 0); 

  if(file_map == MAP_FAILED){
    mmapFail(argv[1]);
    return 2;
  }
  char file_type[5];
  strcpy(file_type, file_map);
  file_type[4] = '\0';
  printf("%s\n", file_type);
  if(0 != strcmp(file_type, "BTRE")){
    printf("invalid file\n");
    return 2;
  }

  void* cur = file_map + 4;
  //loop here
  for(int i = 2; i < argc; i++){
    while(1){
      BinaryTreeNode* node = cur;
      char* word = cur + 16;
      size_t n;
      int go_left;
      if(strlen(argv[i]) > strlen(word)){
        n = strlen(word);
        go_left = 0;
      }else{
        n = strlen(argv[i]);
        go_left = 1;
      }

      int cmp = strncmp(word, argv[i], n);
      if(0 == strcmp(word, argv[i])){
        printFound(word, node->count, node->price);
        break;
      }else if(cmp > 0){
        if(node->left_child == 0){
          printNotFound(argv[i]);
          break;
        }
        cur = file_map + node->left_child;
      }else if(cmp < 0){
        if(node->right_child == 0){
          printNotFound(argv[i]);
          break;
        }
        cur = file_map + node->right_child;
      }else{
        if(go_left){
          if(node->left_child == 0){
            printNotFound(argv[i]);
            break;
          }
          cur = file_map + node->left_child;
        }else if(!go_left){
          if(node->right_child == 0){
            printNotFound(argv[i]);
            break;
          }
          cur = file_map + node->right_child;
        }
      }
    }
    cur = file_map + 4;
  }

  close(fp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double total = (end.tv_sec-start.tv_sec)+((double)(end.tv_nsec-start.tv_nsec)/1000000000);
  printTime(total);
  return 0; 
}
