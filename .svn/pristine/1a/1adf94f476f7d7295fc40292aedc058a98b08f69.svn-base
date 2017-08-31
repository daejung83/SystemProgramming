/**
 * MapReduce
 * CS 241 - Spring 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"
#include "libds.h"

void usage() {
  fprintf(stderr, "shuffler destination1 destination2 ...\n");
  fprintf(stderr, "where destination1..n are files on the filesystem to which "
                  "the shuffler will write its output\n");
}

int main(int argc, char *argv[]) {
  // read from stdin
  // hash the key for the input line
  // send them to the correct output file (output files are given as command
  // line arguments
  if (argc < 2) {
    usage();
    exit(1);
  }
	int file[argc-1];
  for(int i = 0; i < argc-1; i++){
  
    file[i] = open(argv[i+1], O_WRONLY);
		if(file[i] < 0){
			fprintf(stderr, "open failed file[%d]: %s in shuffler\n", i+1, argv[i+1]);
			exit(1);
		}
  }

	for(int i = 1; i <= argc; i++){
		char* line = NULL;
		size_t len = 0;
		ssize_t error;
    char* temp_line = NULL;
		while((error = getline(&line, &len, stdin)) != -1){
			char* key;
			char* val;
			//line[error] = '\0';
			//printf("line[error]: %c\n", line[error-1]);
      temp_line = strdup(line);
      //printf("line before: %s\n", line);
			split_key_value(temp_line, &key, &val);
      int hash_key = hashKey(key)%(argc-1);
      line[error-1] = '\n';
      line[error] = '\0';
			write(file[hash_key], line, strlen(line));
      //write(file[hash_key], "\n", 1);
      //printf("line: %s hash: %lu\n", line, sizeof(line));
		}
      free(temp_line);
	  free(line);		
	}
	for(int i = 0; i < argc-1; i++){
    close(file[i]);
	}
  return 0;
}

