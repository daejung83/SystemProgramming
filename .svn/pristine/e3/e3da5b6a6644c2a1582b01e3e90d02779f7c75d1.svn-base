#include "editor.h"
#include "document.h"
#include "format.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX(a, b) (((a)>(b))?(a):(b))
#define MIN(a, b) (((a)<(b))?(a):(b))


char *get_filename(int argc, char *argv[]) {
  // TODO implement get_filename
  // take a look at editor_main.c to see what this is used for
  
  return argv[argc - 1];
}

void handle_display_command(Document *document, const char *command) {
  // TODO implement handle_display_command
  if(Document_size(document) == 0){
    print_document_empty_error();
    return;
  }
  if(strlen(command) == 1){
    for(size_t i = 0; i < Document_size(document); i++){
      print_line(document, i);
    }
    return;
  }
  
  if(command[2] == ' '){
    invalid_command(command);
    return;
  }
  if(command[1] != ' '){
    invalid_command(command);
    return;
  }
  char org_command[strlen(command)+1];
  strcpy(org_command, command);
  
  size_t doc_size = Document_size(document);
  command = command + 2;
  size_t line_num = atoi(command);
  if(doc_size  < line_num || line_num < 1){
    invalid_line();
    return;
  }

  while(*command != '\0'){
    if(!isdigit(*command)){
      invalid_command(org_command);
      return;
    }
    command++;
  }


  size_t min;
  if(line_num < 6)
    min = 0;
  else
    min = line_num - 6;

  size_t max = MIN(doc_size, line_num + 5); 

  for(size_t i = min; i < max; i++){
    print_line(document, i); 
  }
}

void handle_write_command(Document *document, const char *command) {
  // TODO implement handle_write_command
    
  if(command[2] == ' '){
    invalid_command(command);
    return;
  }
  //size_t doc_size = Document_size(document);
  char org_command[strlen(command)+1];
  strcpy(org_command, command);
  command = command + 2;
  size_t line_num = atoi(command);
  //printf("%zu\n", line_num);
  if(line_num < 1){
    invalid_line();
    return;
  }
  while(*command != ' '){
    command++;
    if(*command == '\0'){
      invalid_command(org_command);
      return;
    }
  }
  command++;

  size_t tok = 0;
  //printf("command %s\n", command);
  char** input_split = strsplit(command, "$", &tok); 
  //printf("tok %zu\n", tok); 
  Document_set_line(document, line_num-1, input_split[0]);
  for(size_t i = 1; i < tok; i++){
    Document_insert_line(document, line_num+i-1, input_split[i]);
  }
  //Document_set_line(document, line_num-1, command);
  for(size_t i = 0; i < tok; i++){
    free(input_split[i]);
  }
  free(input_split);
}

void handle_append_command(Document *document, const char *command) {
  // TODO implement handle_append_command
  
  
  if(command[2] == ' '){
    invalid_command(command);
    return;
  }
  char org_command[strlen(command)+1];
  strcpy(org_command, command);
  //size_t doc_size = Document_size(document);
  command = command + 2;
  size_t line_num = atoi(command);
  if(line_num < 1){
    invalid_line();
    return;
  }
  while(*command != ' '){
    if(*command == '\0'){
      invalid_command(org_command);
      return;
    }
    command++;
    //system("sleep 1");
  }
  command++;
  size_t tok = 0;
  //printf("command %s\n", command);
  char** input_split = strsplit(command, "$", &tok); 
  //printf("tok %zu\n", tok);
  char first_input[strlen(input_split[0] + strlen(Document_get_line(document, line_num))) + 1];
  strcpy(first_input, Document_get_line(document, line_num-1));
  strcat(first_input, input_split[0]);
  //printf("first_input: %s\n", first_input);
  Document_set_line(document, line_num-1, first_input);
  for(size_t i = 1; i < tok; i++){
    Document_insert_line(document, line_num+i-1, input_split[i]);
  }
  //free input_split
  for(size_t i = 0; i < tok; i++){
    free(input_split[i]);
  }
  
  free(input_split);
}

void handle_delete_command(Document *document, const char *command) {
  // TODO implement handle_delete_command
  if(command[2] == ' '){
    invalid_command(command);
    return;
  }
  char org_command[strlen(command)+1];
  strcpy(org_command, command);
  size_t doc_size = Document_size(document);
  command = command + 2;
  size_t line_num = atoi(command);
  if(doc_size  < line_num || line_num < 1){
    invalid_line();
    return;
  }
  while(*command != '\0'){
    if(!isdigit(*command)){
      invalid_command(org_command);
      return;
    }
    command++;
  }
  Document_delete_line(document, line_num-1);
}

void handle_search_command(Document *document, const char *command) {
  // TODO implement handle_search_command
  //if(command[2] == ' '){
  //  invalid_command(command);
  //  return;
  //}
  size_t doc_size = Document_size(document);
  command = command + 1;
  //size_t line_num = atoi(command);
  //if(doc_size  < line_num || line_num < 1){
  //  invalid_line();
  //  return;
  
  for(size_t i = 0; i < doc_size; i++){
    char* find = strstr(Document_get_line(document, i), command);
    if(find != NULL){
      print_search_line((int)i+1, Document_get_line(document, i), find, command);
    }
  }
  
  //for(size_t i = 0; i < doc_size; i++){
  //  //size_t temp_search_len = search_len;
  //  size_t line_size = strlen(Document_get_line(document, i));
  //  char* start = NULL;
  //  if(line_size >= search_len ){
  //    char doc_line[line_size+1]; 
  //    strcpy(doc_line, Document_get_line(document, i));

  //    for(size_t j = 0; j < line_size; j++){
  //      //printf("char %c\n", doc_line[j]);
  //      //if first letter found
  //      if(doc_line[j] == command[0]){
  //        //loop to see if rest are correct
  //        start = &doc_line[j];
  //        for(size_t ii = 1; ii < search_len; ii++){
  //          j++;
  //          //if
  //          if(doc_line[j] != command[ii]){
  //            start = NULL;
  //            break;
  //          }
  //        }
  //      }
  //    }
  //    if(start != NULL)
  //      print_search_line((int)i + 1, doc_line, start, command);
  //  }
  //}
  
  //void print_search_line(int line_number, const char *line, const char *start,
}

void handle_save_command(Document *document, const char *filename) {
  // TODO implement handle_save_command
  Document_write_to_file(document, filename);
}
