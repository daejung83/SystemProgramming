/**
 * Et Tu, Brute Lab
 * CS 241 - Spring 2016
 */

#include "cipher.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

/**
 * This function will parse your program arguments for you.

 * When it is done it will return an array of void pointers.
 * Suppose that void ** args = arg_parse(int argc, char *argv[]):
 *  args[0] - is a pointer to the number of lines in the file
 *  args[1] - is a pointer to the number of bytes in the longest line
 *  args[2] - is a FILE pointer to the input file
 *  args[3] - is a FILE pointer to the output file
 *
 * Features:
 *  Will prompt the user if they don't have the right arguments
 *  Will prompt the user if the input file is not readable
 *  Will prompt the user if the output file is not writable
 *  Will rewind() the FILE pointer for the input file
 *
 * Note: When you are done using the args returned
 *  Call free_args to clean up memory and close files
 */
void **arg_parse(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "%s <input_file> <output_file>\n", argv[0]);
    exit(1);
  }

  // Testing that the input and output files are valid ...
  char *input_file_name = argv[1];
  FILE *input_file = fopen(input_file_name, "r");
  if (input_file == NULL) {
    fprintf(stderr, "Was not able to read from the input file. Exiting...\n");
    exit(1);
  }

  char *output_file_name = argv[2];
  FILE *output_file = fopen(output_file_name, "w");
  if (output_file == NULL) {
    fprintf(stderr, "Was not able to write to the output file. Exiting...\n");
    exit(1);
  }

  // Open input file
  //  -> figure out how many lines it has and how long the longest line is
  char *line = NULL;
  size_t len = 0;
  ssize_t bytes_read;
  size_t *num_lines = calloc(1, sizeof(size_t));
  size_t *max_line_length = calloc(1, sizeof(size_t));

  while ((bytes_read = getline(&line, &len, input_file)) != -1) {
    // Figuring out how many lines the file has
    (*num_lines)++;
    // Updating the max line length
    *max_line_length = *max_line_length > (size_t)bytes_read
                           ? *max_line_length
                           : (size_t)bytes_read;
  }
  free(line);
  rewind(input_file); // move pointer back to begining of the file

  // Ok return all this information to the user

  // One void pointer for each of the return values
  void **retvals = malloc(4 * sizeof(void *));
  retvals[0] = num_lines;
  retvals[1] = max_line_length;
  retvals[2] = input_file;
  retvals[3] = output_file;
  return retvals;
}

// Call this function to free the args returned from arg_parse()
void free_args(void **args) {
  free(args[0]);   // Freeing size_t *num_lines
  free(args[1]);   // Freeing size_t *max_line_F_SETPIPE_SZlength
  fclose(args[2]); // Closing the input file
  fclose(args[3]); // Closing the output file
  free(args);      // Freeing the args array
}

int main(int argc, char *argv[]) {
  // Alea iacta est
  void** result = arg_parse(argc, argv);
  int pipe_size = *((size_t*)result[1]);
  //int pipe_line[2];


  FILE* fp = result[2];
  char* line = NULL;
  size_t len = 0;
  size_t line_num = 0;
  ssize_t reads = 0;
  char buf[pipe_size];

  pid_t pid[*(size_t*)result[0]];
  //printf("pid: %d\n", pid);
  char* output[*(size_t*)result[0]]; 
  char* new_line;
  int status;
  int pipe_line[*(size_t*)result[0]][2];
  while((reads = getline(&line, &len, fp)) != -1){
    //printf("loop\n");
    //signal
    //new_line = get_most_likely_print_out(line);
    //printf("new_line: %s\n", new_line);
    //output[line_num] = new_line;
    //line_num++;
    //end of signal

  if(pipe(pipe_line[line_num]) == -1){
    perror("pipe failed\n");
    return EXIT_FAILURE;
  }
  fcntl(pipe_line[line_num][1], F_SETPIPE_SZ, &pipe_size);
    pid[line_num] = fork();
    //printf("before if\n");
    if(pid[line_num] == 0){
      //child do later
      //printf("child: %s\n", line);
      line[strlen(line) -1] = '\0';
      new_line = get_most_likely_print_out(line);
      //new_line[strlen(new_line)] = '\0';
      write(pipe_line[line_num][1], new_line, pipe_size);
      close(pipe_line[line_num][1]);
      exit(0);
    }else if(pid[line_num] > 0){
    }else{
      //fork failed
      fprintf(stderr, "fork failed\n");
      return EXIT_FAILURE;
    }
    line_num++;
  } 
  for(size_t i = 0; i < *(size_t*)result[0]; i++){
    read(pipe_line[i][0], buf, sizeof(buf)); 
    output[i] = strdup(buf);
    close(pipe_line[i][0]);
  }
  for(size_t i = 0; i < *(size_t*)result[0]; i++){
    waitpid(pid[i], &status, WNOHANG);
  }

  //file write at the end
  FILE* fp_out = result[3];

  for(size_t i = 0; i < *(size_t*)result[0]; i++){
    fprintf(fp_out,"%s\n" , output[i]);
  }

  free_args(result);
  return 0;
}
