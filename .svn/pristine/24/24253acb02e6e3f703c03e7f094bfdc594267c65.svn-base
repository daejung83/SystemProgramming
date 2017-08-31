/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "common.h"

void usage() {
  printf("./mr2 input_file output_file mapper_exec reducer_exec num_mappers num_shuffler\n");
}

int main(int argc, char **argv) {
  if(argc < 7){
    usage();
    exit(1);
  }
  int num_mapper = atoi(argv[5]);
	int num_shuffler = atoi(argv[6]);
  // Create an input pipe for each mapper.
  // Create one input pipe for the reducer.
  pid_t pid[num_mapper];
	pid_t pid_mapper[num_mapper];
  pid_t pid_reducer[num_shuffler];
	pid_t pid_shuffler;
  int pipe_lines[num_mapper][2];
  //int pipe_lines2[num_maps][2];
  int new_pipe[2];
  //int status = 0;
  //int status_map = 0;
  int status_red = 0;
  for(int i = 0; i < num_mapper; i++){
    if(pipe2(pipe_lines[i], O_CLOEXEC) != 0){
      fprintf(stderr, "pipe failed\n");
      exit(1);
    }
    descriptors_add(pipe_lines[i][0]);
    descriptors_add(pipe_lines[i][1]);
  }
  if(pipe2(new_pipe, O_CLOEXEC) != 0){
    fprintf(stderr, "pipe failed\n"); 
    exit(1);
  }
  descriptors_add(new_pipe[0]);
  descriptors_add(new_pipe[1]);
  // Open the output file.
  int out = open(argv[2], O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
  char command[100];
  strcpy(command, "0");
  //splitter ***********************************
  for(int i = 0; i < num_mapper; i++){
    pid[i] = fork();
    if(pid[i] == 0){
      //baby
      //printf("%s\n", command);
      if(dup2(pipe_lines[i][1], 1) < 0){
        fprintf(stderr, "dup2 failed out spliter\n");
        exit(1);
      }
      char* splitter = "./splitter";
      execl(splitter, splitter, argv[1], argv[5], command, (char*)NULL);
      fprintf(stderr, "execl splitter failed\n");
      exit(1);
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


	//mapper ********************
  for(int i = 0; i < num_mapper; i++){
    pid_mapper[i] = fork();
    if(pid_mapper[i] == 0){
      if(dup2(new_pipe[1], 1) < 0 ){
      	fprintf(stderr, "dup2 failed in mapper\n");
      	exit(1);
    	}
      if(dup2(pipe_lines[i][0], 0) < 0){
      	fprintf(stderr, "dup2 failed out mapper\n");
      	exit(1);
    	}

      execl(argv[3], argv[3], (char*)NULL);
      exit(42);
    }
  }

	//start of shuffler***********************
	char* fifo_files[num_shuffler + 1];
	char fifo_temp[num_shuffler + 1][20];
	
	//snprintf(fifo_files[0], sizeof(fifo_files[0]), "./shuffler");
	//strcpy(fifo_files[0], "./shuffler");
	fifo_files[0] = "./shuffler";	
	for(int i = 1; i <= num_shuffler; i++){
  	snprintf(fifo_temp[i], sizeof(fifo_temp[i]), "fifo_file%d.txt", i);
		fifo_files[i] = fifo_temp[i];
	}
	fifo_files[num_shuffler+1] = NULL;
	char* shuf = "./shuffler";
	//int status_shuf;

	for(int i = 0; i < num_shuffler; i++){
		remove(fifo_files[i+1]);
		int mkerror = mkfifo(fifo_files[i+1], S_IRWXU);
		//printf("made mkfifo %s\n", fifo_files[i+1]);
		if(mkerror < 0){
			fprintf(stderr, "mkfifo failed\n");
			exit(1);
		}
	}
	pid_shuffler = fork();
	if(pid_shuffler == 0){
		//baby
		if(dup2(new_pipe[0], 0) < 0){
			fprintf(stderr, "dup2 failed in shuffler\n");
			exit(1);		
		}
		execv(shuf, fifo_files);
		fprintf(stderr, "execv shuffler failed\n");
		exit(1);
	}else if(pid_shuffler > 0){
		//mommy
	}else{
		//fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
				
	int fifo_fd[num_shuffler];

	//for(int i = 0; i < num_shuffler; i++){
  //  printf("blah\n");
	//	fifo_fd[i] = open(fifo_files[i+1], O_RDONLY);
	//	if(fifo_fd[i] < 0){
	//		fprintf(stderr, "fifo_fd[%d] failed to open\n", i);
	//		exit(1);
	//	}
	//}	
	//new_pipe needs to be changed
	//reducer ********************
	for(int i = 1; i <= num_shuffler; i++){	
  	pid_reducer[i-1] = fork();
  	if(pid_reducer[i-1] == 0){
			fifo_fd[i-1] = open(fifo_files[i], O_RDONLY);
			descriptors_add(fifo_fd[i-1]);
  	  if(dup2(out, 1) < 0){
  	    fprintf(stderr, "dup2 failed reducer out\n");
  	    exit(1);
  	  }

  	  if(dup2(fifo_fd[i-1], 0) < 0){
  	    fprintf(stderr, "dup2 failed reducer in i:%d\n", i);
  	    exit(1);
  	  }
  	  execl(argv[4], argv[4], (char*)NULL);
  	  fprintf(stderr, "should not see this\n");
  	  exit(1);
  	}else if(pid_reducer[i-1] > 0){
      //waitpid(pid_reducer[i-1], &status_red, WEXITED | WSTOPPED);
  	}else{
  	  fprintf(stderr, "fork failed\n");
  	  exit(1);
  	}
	}
  descriptors_closeall();
  descriptors_destroy();
  close(out);
  //for(int i = 0; i < num_mapper; i++){
  //  //waitpid(pid[i], &status_map, 0);
  //  waitpid(pid_mapper[i], &status_map, 0);
  //  if(!WIFEXITED(status_map) && WEXITSTATUS(status_map)){
  //    printf("%s %d exited with status %d\n", argv[3], i + 1, WEXITSTATUS(status_map)); 
  //  }
  //}
  for(int i = 0; i < num_shuffler; i++){
		waitpid(pid_reducer[i], &status_red, 0);
    //unlink(fifo_files[i+1]);
    if(!WIFEXITED(status_red) && WEXITSTATUS(status_red)){
      printf("%s %d exited with status %d\n", argv[4], i + 1, WEXITSTATUS(status_red)); 
    }
  }
  //waitpid(pid_shuffler, &status_shuf, 0);




  char* line = NULL;
  //char line[1024];
  size_t len;
  int counter = 0;
  //char touch_buff[200];
  //snprintf(touch_buff, sizeof(touch_buff), "touch %s", argv[2]);
  //system(touch_buff);
  //system("cat output.txt");
  FILE* fp = fopen(argv[2], "r");
  //FILE* fp = fdopen(out, "r");
  //printf("fp:  %p\n", fp);
  fseek(fp, 0, SEEK_SET);
  while(getline(&line, &len, fp) != -1){
    //printf("balhbsaljdkhgdlaks\n");
    counter++;
  }

  free(line);
  fclose(fp);
  printf("output pairs in %s: %d\n", argv[2], counter);
  return 0;
}
