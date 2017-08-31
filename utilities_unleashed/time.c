#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include "format.h"

#define PATH "/usr/bin/"

int main(int argc, char *argv[]) {
    //argv[1] = command tag

    struct timespec start, end;
    
    if(argc <= 1)
      return 0;

    char * string = malloc(strlen(PATH) + strlen(argv[1]) +2);
    strcpy(string, PATH);
    strcat(string, argv[1]);

    int status = 1;
  
    clock_gettime(CLOCK_MONOTONIC, &start);
    //testing fork
    pid_t pid = fork();
    if(pid ==  0){
      if(execvp(string, argv+1) == -1)
        exit(3);
      else
        exit(0);
    }else if (pid > 0){
      waitpid(pid, &status, 0);
      if(WIFEXITED(status)){
        if(!WEXITSTATUS(status)){
        //if(WEXITSTATUS(status)){
          clock_gettime(CLOCK_MONOTONIC, &end);
          double total = (end.tv_sec-start.tv_sec)+((double)(end.tv_nsec-start.tv_nsec)/1000000000);
          display_results(argv, total);
        //}
        }else{
          print_exec_failed();
        }
      }else{
        print_exec_failed();
      }
    }else{
      print_fork_failed();
    }
    //if(pid > 0){
    //  waitpid(pid, &status, 0);
    //  printf("status: %d\n", WEXITSTATUS(status));
    //  if(WIFEXITED(status)){ 
    //      if(WEXITSTATUS(status)){
    //      printf("status is okay\n");
    //      clock_gettime(CLOCK_MONOTONIC, &end);
    //      uint64_t total = (end.tv_sec-start.tv_sec)+((end.tv_nsec-start.tv_nsec)/100000000);
    //      printf("%s took total of %lu s\n", *argv, total);
    //  
    //      } 
    //  }else{
    //    printf("waitpid failed to returning\n");
    //    return -1;
    //  }
    //  //print failed
    //}else if(pid == 0){
    //  printf("hello woirld\n");
    //  int x = execvp(string, argv+1);
    //  printf("%d\n", x);
    //  //if(execvp(string, argv+1) == -1)
    //  //  exit(0);
    //  //else
    //    exit(3);
    //}else{
    //  //fork failed
    //  printf("fork failed\n");
    //  return -1;
    //}

    return 1;
}
