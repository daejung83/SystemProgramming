/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

void usage() {
  printf("./mr1 input_file output_file mapper_exec reducer_exec num_mappers\n");
}

int main(int argc, char **argv) {
  if(argc < 6){
    usage();
    exit(1);
  }
  int num_maps = atoi(argv[5]);
  // Create an input pipe for each mapper.
  // Create one input pipe for the reducer.
  pid_t pid[num_maps];
  pid_t pid2[num_maps];
  int pipe_lines[num_maps][2];
  //int pipe_lines2[num_maps][2];
  int new_pipe[2];
  int status = 0;
  int status_map = 0;
  int status_red = 0;
  pid_t pid_set;
  //int link_pipe[2];
  //if(pipe(link_pipe) != 0){
  //  fprintf(stderr, "pipe failed\n");
  //  exit(1);
  //}
  for(int i = 0; i < num_maps; i++){
    if(pipe2(pipe_lines[i], O_CLOEXEC) != 0){
      fprintf(stderr, "pipe failed\n");
      exit(1);
    }
    descriptors_add(pipe_lines[i][0]);
    descriptors_add(pipe_lines[i][1]);
    //if(pipe(pipe_lines2[i]) != 0){
    //  fprintf(stderr, "pipe failed\n");
    //  exit(1);
    //}
  }
  if(pipe2(new_pipe, O_CLOEXEC) != 0){
    fprintf(stderr, "pipe failed\n"); 
    exit(1);
  }
  descriptors_add(new_pipe[0]);
  descriptors_add(new_pipe[1]);
  // Open the output file.
  int out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  //descriptors_add(out);
  // Start a splitter process for each mapper.
  char command[100];
  //char command[10];
  //strcpy(command, "./splitter \0");
  //strcpy(command, argv[2]);
  //strcat(command, " ");
  //strcat(command, argv[5]);
  strcpy(command, "0");
  for(int i = 0; i < num_maps; i++){
    pid[i] = fork();
    if(pid[i] == 0){
      //baby
      //printf("%s\n", command);
      if(dup2(pipe_lines[i][1], 1) < 0){
        fprintf(stderr, "dup2 failed\n");
        exit(1);
      }
      char* splitter = "./splitter";
      execl(splitter, splitter, argv[1], argv[5], command, (char*)NULL);
      //fprintf(stderr, "should not see this\n");
      exit(42);
    }else if(pid[i] > 0){
      //mommy
    }else{
      //fork failed
      fprintf(stderr, "fork failed\n");
      exit(1);
    }
    //printf("command: %s,\n", command);
    command[strlen(command)-1] = command[strlen(command)-1] + 1;
  }
  // Start all the mapper processes.

  //char buffer[1024];
  //for(int i = 0; i < num_maps; i++){
  //  close(pipe_lines[i][1]); 
  //  int n = read(pipe_lines[i][0], buffer, 1024);
  //  buffer[n] = '\0';
  //  //printf("test: %s\n", buffer);
  //  write(new_pipe[1], buffer, strlen(buffer));   
  //  close(pipe_lines[i][0]);
  //}
  //int dup_status;
    for(int i = 0; i < num_maps; i++){
      //printf("first loop i: %d\n", i);
      pid2[i] = fork();
      if(pid2[i] == 0){
        if(dup2(new_pipe[1], 1) < 0 ){
          fprintf(stderr, "dup2 failed\n");
          exit(1);
        }
        if(dup2(pipe_lines[i][0], 0) < 0){
          fprintf(stderr, "dup2 failed\n");
          exit(1);
        }

        execl(argv[3], argv[3], (char*)NULL);
        exit(42);
      }
    }
  //else{
  //  waitpid(dup_pid, &dup_status, WEXITED | WSTOPPED);
  //}

  //char buffer[1024];
  //for(int i = 0; i < num_maps; i++){
  ////printf("hello\n");

  //  close(pipe_lines[i][1]); 
  //  close(pipe_lines[i][0]);
  //  close(pipe_lines2[i][1]);
  //  int n = read(pipe_lines2[i][0], buffer, 1024);
  //  printf("n: %d\n", n);
  //  buffer[n] = '\0';
  //  //printf("test: %s\n", buffer);
  //  write(new_pipe[1], buffer, strlen(buffer));   
  //  close(pipe_lines2[i][0]);
  //}
  
  //close(new_pipe[1]);

  //for(int i = 0; i < num_maps; i++){
  //  waitpid(pid[i], &status, WUNTRACED | WCONTINUED); 
  //  if(WIFEXITED(status) && WEXITSTATUS(status))
  //    printf("%s %d exited with status %d\n", argv[3], i + 1, WEXITSTATUS(status));
  //}

  //int n = read(new_pipe[0], buffer, 1024);
  //buffer[n] = '\0';
  //printf("%s\n", buffer);
  // Start the reducer process.
  //pid_set[0] = fork();

  //if(pid_set[0] == 0){
  //  if(dup2(new_pipe[0], 0) < 0){
  //    fprintf(stderr, "dup2 failed\n");
  //    exit(1);
  //  }
  //  if(dup2(link_pipe[1], 1) < 0){
  //    fprintf(stderr, "dup2 failed\n");
  //    exit(1);
  //  }
  //  close(link_pipe[0]);
  //  close(link_pipe[1]);
  //  close(new_pipe[0]);
  //  close(new_pipe[1]);
  //  execl(argv[3], argv[3], (char*)NULL);
  //  fprintf(stderr, "should not see this\n");
  //  exit(1);
  //}else if(pid_set[0] > 0){

  //}else{
  //  fprintf(stderr, "fork failed\n");
  //  exit(1);
  //}
  // Wait for the reducer to finish.
  
  //char buffer[1024];
  //int n = read(new_pipe[0], buffer, 1024);
  //buffer[n] = '\0';
  //printf("test: %s\n", buffer);

  pid_set = fork();
  if(pid_set == 0){
    if(dup2(new_pipe[0], 0) < 0){
      fprintf(stderr, "dup2 failed\n");
      exit(1);
    }
    if(dup2(out, 1) < 0){
      fprintf(stderr, "dup2 failed\n");
      exit(1);
    }
    execl(argv[4], argv[4], (char*)NULL);
    fprintf(stderr, "should not see this\n");
    exit(42);
  }else if(pid_set > 0){

  }else{
    fprintf(stderr, "fork failed\n");
    exit(1);
  }
  for(int i = 0; i < num_maps; i++){
    waitpid(pid[i], &status_map, WEXITED | WSTOPPED);
    waitpid(pid2[i], &status_map, WEXITED | WSTOPPED);
    if(WIFEXITED(status_map) && WEXITSTATUS(status)){
      printf("%s %d exited with status %d\n", argv[3], i + 1, WEXITSTATUS(status)); 
    }
  }
  waitpid(pid_set, &status_red, WEXITED | WSTOPPED);
  //close(link_pipe[0]);
  //close(link_pipe[1]);
  //close(new_pipe[0]);
  //close(new_pipe[1]);
  descriptors_closeall();
  descriptors_destroy();
  //close(out);
  // Print nonzero subprocess exit codes.
  //printf("%s exited with status %d\n", argv[3], status_map);
  //printf("%s exited with status %d\n", argv[4], status_red);

  // Count the number of lines in the output file.
  char* line = NULL;
  //char line[1024];
  size_t len;
  int counter = 0;
  close(out);
  char touch_buff[200];
  snprintf(touch_buff, sizeof(touch_buff), "touch %s", argv[2]);
  system(touch_buff);
  //system("cat output.txt");
  FILE* fp = fopen(argv[2], "r+");
  //printf("fp:  %p\n", fp);
  //fseek(fp, 0, SEEK_SET);
  while(getline(&line, &len, fp) != -1){
    counter++;
  }

  free(line);
  fclose(fp);
  printf("output pairs in %s: %d\n", argv[2], counter);
  return 0;
}
