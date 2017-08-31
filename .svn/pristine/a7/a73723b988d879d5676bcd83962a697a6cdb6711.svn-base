/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include "common.h"

void usage() {
  printf("./mr0 input_file output_file mapper_exec reducer_exec\n");
}

int main(int argc, char **argv) {
  if(argc < 4){
    usage();
    exit(1);
  }
  // Open the input file.
  //FILE* fp = fopen(argv[1], "r"); 
  int in = open(argv[1], O_RDONLY);
  // Create a pipe to connect the mapper to the reducer.
  int fd[2];
  if(pipe(fd) != 0){
    fprintf(stderr, "pipe failed\n");
    exit(1);
  }
  // Open the output file.
  //FILE* fp_out = fopen(argv[2], "w+");
  int out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  // Start the mapper.
  pid_t pid[2]; 
  int status_mapper;
  int status_reducer;
  pid[0] = fork();
  if(pid[0] == 0){
    //baby
    //printf("mapper fork\n");
    //char* temp = "string";
    //printf("argv[3]: %s\n", argv[3]);
    if(dup2(in, 0) < 0){
      fprintf(stderr, "dup2 failed: mapper in\n");
      exit(1);
    }
    if(dup2(fd[1], 1) < 0){
      fprintf(stderr, "dup2 failed: mapper out\n");
      exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    close(out);
    close(in);
    execl(argv[3], argv[3], (char*)NULL);   
    fprintf(stderr, "should not see line\n");
    exit(1);
  }else if(pid[0] > 0){
    //mommy
  }else{
    //failed
    fprintf(stderr, "fork for mapper failed\n");
    exit(1);
  }
  // Start the reducer.
  pid[1] = fork();
  if(pid[1] == 0){
    //baby
    if(dup2(fd[0], 0) < 0){
      fprintf(stderr, "dup2 failed: reducer in\n");
      exit(1);
    }
    if(dup2(out, 1) < 0){
      fprintf(stderr, "dup2 failed: reducer out\n");
      exit(1);
    }
    close(fd[0]);
    close(fd[1]);
    close(out);
    close(in);
    execl(argv[4], argv[4], (char*)NULL);
    //fprintf(stderr, "should not see line\n");
    exit(2);
  }else if(pid[1] > 0){
    //mommy
  }else{
    //fork failed
    fprintf(stderr, "fork for reducer failed\n");
    exit(1);
  }
  // Wait for the reducer to finish.
  waitpid(pid[0], &status_mapper, 0);//WEXITED | WSTOPPED); 
  waitpid(pid[1], &status_reducer, 0);//WEXITED | WSTOPPED); 
  // Print nonzero subprocess exit codes.
  close(fd[0]);
  close(fd[1]);
  if(WIFEXITED(status_mapper))
    printf("%s exited with status %d\n", argv[3], WEXITSTATUS(status_mapper));
  if(WIFEXITED(status_reducer))
    printf("%s exited with status %d\n", argv[4], WEXITSTATUS(status_reducer));
  // Count the number of lines in the output file.
  close(in);
  if(close(out) != 0){
    fprintf(stderr, "file close failed\n");
    exit(1);
  }
  char* line = NULL;
  size_t len;
  int counter = 0;
  //system("cat output.txt");
  char touch_buff[200];
  snprintf(touch_buff, sizeof(touch_buff), "touch %s", argv[2]);
  system(touch_buff);
  FILE* fp = fopen(argv[2], "r");
  while(getline(&line, &len, fp) != -1){
    counter++;
  }
  printf("output pairs in %s: %d\n", argv[2], counter);
  return 0;
}
