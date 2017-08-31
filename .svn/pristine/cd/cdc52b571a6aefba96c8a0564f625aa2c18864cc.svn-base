/**
 * Mad Mad Access Pattern
 * CS 241 - Spring 2016
 */
#include <time.h>
#include "tree.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
  Look up a few nodes in the tree and print the info they contain.
  This version uses fseek() and fread() to access the data.

  ./lookup1 <data_file> <word> [<word> ...]
*/

int main(int argc, char **argv) {
  struct timespec start, end;
  clock_gettime(CLOCK_MONOTONIC, &start); 
  if(argc < 3){
    printf("\n\tlookup1 <data_file> <word> [<word> ...]\n\n");
    return 1;
  }
  FILE* fp = fopen(argv[1], "r");
  if(!fp){
    //printf("Cannot open file %s\n", argv[1]);
    openFail(argv[1]);
    return 2;
  }
  char file_type[4];
  fread(file_type, 4, 1, fp);
  //printf("%s\n", file_type);
  if(0 != strcmp(file_type, "BTRE")){
    //invalid file
    printf("invalid file\n");
    return 2;
  }
  char buffer[200];
  //fseek(fp, sizeof(BinaryTreeNode), SEEK_SET);
  size_t cur_node = 4;
  for(int i = 2; i < argc; i ++){
    while(1){
  	  fseek(fp, cur_node, SEEK_SET); 
  	  BinaryTreeNode* temp = malloc(sizeof(BinaryTreeNode));
  	  fread(temp, sizeof(BinaryTreeNode), 1, fp);
  	  //printf("left_child: %u\n", temp->count);
  	  fseek(fp, cur_node, SEEK_SET); 
  	  fseek(fp, 16, SEEK_CUR);
  	  fread(buffer, 200, 1, fp);
  	  //printf("buffer %s,\n", buffer);

      size_t n;
      int go_left;
      if(strlen(argv[i]) > strlen(buffer)){
        n = strlen(buffer);
        go_left = 0;
      }else{
        n = strlen(argv[i]);
        go_left = 1;
      }
      int cmp = strncmp(buffer, argv[i], n);
      //printf("cmp %d\n", cmp);
      //when argv is bigger cmp is neg

  	  if(0 == strcmp(buffer, argv[i])){
    	  printFound(buffer, temp->count, temp->price); 
        break;
  	  }else if(cmp > 0){
			  if(temp->left_child == 0){
          printNotFound(argv[i]); 
          break;
        }
        cur_node = temp->left_child;
		  }else if(cmp < 0){
        if(temp->right_child == 0){
          printNotFound(argv[i]);
          break;
        }
        cur_node = temp->right_child;
      }else{
        //printNotFound(argv[i]);
        if(go_left){
          if(temp->left_child == 0){
            printNotFound(argv[i]);
            break;
          }
          cur_node = temp->left_child;
        }else if(!go_left){
          if(temp->right_child == 0){
            printNotFound(argv[i]);
            break;
          }
          cur_node = temp->right_child;
        }
        //break;
      }
    }
    cur_node = 4;
  }
  fclose(fp);
  clock_gettime(CLOCK_MONOTONIC, &end);
  double total = (end.tv_sec-start.tv_sec)+((double)(end.tv_nsec-start.tv_nsec)/1000000000);
  printTime(total);
  
  return 0; 
}

