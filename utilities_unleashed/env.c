#define _GNU_SOURCE
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "format.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>
#define PATH "/usr/bin/"

int main(int argc, char *argv[])
{
  //char** evn = __environ;
  int status = 0;

  if(argc == 2){
    print_env_usage();
    return 0;
  }
  pid_t pid = fork();

  //fork processes
  if(pid == 0){
    //child
    //run code

      if(argc == 1){
        //might need to move env ptr to print
        system("env");
        return 0;
      }  //split string work start 
      char* command = (char*)malloc(255);
      strcpy(command, PATH);
      strcat(command, argv[2]);
  
      char* var_st = (char*)malloc(strlen(argv[1]+1));
      strcpy(var_st, argv[1]);
      //printf("var_st: %s\n", var_st);
      
      size_t tok_used = 0;
      size_t tok_cap = 1;
      char** tok_array = calloc(1, sizeof(char*));
      char* tok = NULL;
      char* delim = ",";
      char* str = var_st;
      tok = strsep(&str, delim);
      int track = 0;
      while(tok != NULL){
        if(tok_used == tok_cap){
          tok_cap = tok_cap *2;
          tok_array = (char**)realloc(tok_array, tok_cap*sizeof(char*));
        }
        tok_array[tok_used] = (char*)malloc(strlen(tok)+1);
        strcpy(tok_array[tok_used], tok);
        tok_used++;
      
        tok = strsep(&str, delim);
        track++;
      }

      //for(size_t i = 0; i < tok_used; i++){
      //  printf("array: %zu, %s\n", i , tok_array[i]);
      //}
      //comma sep is finished at this point
      ///and placed in array

      //for loop will take care of each array and create commands
      char* break_string = malloc(255);
      char** path_array = (char**)malloc(sizeof(char*)*tok_used);
      char** path_var_array = (char**)malloc(sizeof(char*)*tok_used);
      for(size_t ii = 0; ii < tok_used; ii++){
        strcpy(break_string, tok_array[ii]);
        //printf("starlen: %zu\n", strlen(tok_array[ii]));
        
        //for(size_t j = 0; j < strlen(tok_array[ii]); j++){
        //  printf("char of array %c\n", *break_string++);
        //}
        //could obtain chars from here

        path_array[ii] = malloc(strlen(tok_array[ii]+1));
        path_var_array[ii] = malloc(255);
        strcpy(path_var_array[ii], "");
        strcpy(path_array[ii], "");
        size_t new_start = 0;
        size_t str_size = 0;
        for(size_t j = 0; j < strlen(tok_array[ii]); j++){
          if(*break_string != '='){
            path_array[ii][str_size] = *break_string;
            path_array[ii][str_size+1] = '\0';
            str_size++;
          }else{
            new_start = j;
            break;
          }
          break_string++;
        }
        //printf("path: %s\n", path_array[ii]);
        break_string++; 
        //obtained path with new_start as and break_string as where rest starts
        //printf("start of new %c\n", *break_string);
        for(size_t j = 0; j < strlen(tok_array[ii])-(new_start+1); j++){

          if(*break_string != '%'){
            path_var_array[ii][j] = *break_string;
            path_var_array[ii][j+1] = '\0';
            break_string++;
          }else{
            //obtain new path
            //need to ++ j and break_string here
            //printf("insde for percent\n");
            break_string++;
            j++;
            char* replace_str = malloc(255);
            strcpy(replace_str, "");
            size_t rep_size = 0;
            while(break_string){
              if(ispunct(*break_string)){
                break;
              }else{
                replace_str[rep_size] = *break_string;
                replace_str[rep_size+1] = '\0';
                rep_size++;
              }
              break_string++;
              j++;
            }
            //cat string
            strcat(path_var_array[ii], getenv(replace_str));
            //printf("get HOME %s\n", path_var_array[ii]);
          }
          
        }
        //printf("path_bar_array: %s\n", path_var_array[ii]);
        if(setenv(path_array[ii], path_var_array[ii], 1) == -1)
          print_environment_change_failed();
      }
      
    //end string split work exec commands
    
    //printf("command: %s\n", command);
    if(execvp(command, argv+2) == -1){
      printf("execvp failed\n");
      exit(3);
    }else{
      printf("execvp did not failed\n");
      exit(0);
    }
  }else if(pid > 0){
    //parent
    waitpid(pid, &status, 0);
    //check if waitpid return correct
    if(WIFEXITED(status)){
        if(!WEXITSTATUS(status)){
          //code ran was successful
          printf("code ran okay\n");
        }else{
          print_exec_failed();
        }
    }else{
      print_exec_failed();
    }
    
  }else{
    //fork failed
    print_fork_failed();
  }

  return 0;
}
