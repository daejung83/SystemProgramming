/**
 * Machine Problem: Shell
 * CS 241 - Spring 2016
 */
#include "format.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

void free_all(Log** command_log, Log** log_file, char** command_file, char** log_file_name);
void run_command_line(const char* command, Log* log, char* log_file_name);

//do nothing
void handle_sigint(int signal){
  
  printf("signal: %d\n", signal);
  //printf("\n");
  //print_prompt(get_current_dir_name(), getpid());
}

int main(int argc, char *argv[]) {
  // TODO: This is the entry point for your shell.
  
  //sigignore(SIGINT);
  signal(SIGINT, handle_sigint);
  
  print_shell_owner("ddkim9");

  Log* command_log = NULL;
  char* command_file = NULL;
  char* log_file_name = NULL;
  Log* log_file = NULL;
  
  if( argc > 5){
    print_usage();
    return 0;
  }
  if(argc > 1){
    if(!(0==strcmp(argv[1],"-f") || 0==strcmp(argv[1], "-h"))){
      print_usage();
      return 0;
    }
  
    if(argc > 3 && !(0==strcmp(argv[3],"-f") || 0==strcmp(argv[3], "-h"))){
      print_usage();
    }
    if(argc%2 == 0){
      print_script_file_error(); 
      return 0;
    }
    for(int i = 1; i < argc; i = i + 2){
      //printf("argc[%d]: %s\n", i, argv[i]);
  
      if(strcmp(argv[i], "-f")==0){
        command_file = strdup(argv[i+1]);
      }else{
        //was check above no need to check again
        log_file = Log_create_from_file(argv[i+1]);
        log_file_name = strdup(argv[i+1]);
      }
    }
  }
  if(log_file != NULL && Log_size(log_file) == 0){
    print_no_history_match();
    free_all(&command_log, &log_file, &command_file, &log_file_name);
    return 0;
  }
  //check if load needs to be done
  //if(log_file != NULL && !(Log_size(log_file) == 0)){
  //  printf("testing\n");
    //load it into log
      
  //}
  //load command file also check argc size
  if(command_file != NULL){
    command_log = Log_create_from_file(command_file);   
    //if open
    if(command_log != NULL && Log_size(command_log)==0){  
      print_script_file_error(); 
      free_all(&command_log, &log_file, &command_file, &log_file_name);
      return 0;
    //else
    }
  }


  //if there is no log file create it
  if(log_file == NULL){
    log_file = Log_create();
  }
  //pid_t pid = getpid();
  //printf("printing PID: %d\n", pid);
  //loop command line and exiting
  if(command_file != NULL){  
    for(size_t i = 0; i < Log_size(command_log); i++){
      print_prompt(get_current_dir_name(), getpid());
      printf("%s\n", Log_get_command(command_log, i));
      run_command_line(Log_get_command(command_log, i), log_file, log_file_name);
    }
    //and free all items
    //need to exit if command file is read in
    if(log_file_name != NULL){
      Log_save(log_file, log_file_name); 
    }
    run_command_line("exit", log_file, log_file_name);

    if(log_file_name != NULL){
      Log_save(log_file, log_file_name); 
    }
    free_all(&command_log, &log_file, &command_file, &log_file_name);
    return 0;
  }
    
  char *input = NULL;
  //while loop for ui
  size_t s = 128;
  int control = 1;
  while(control){  //!(0==strcmp(input, "exit"))){
    print_prompt(get_current_dir_name(), getpid());
    input = malloc(s+1);
    getline(&input, &s, stdin);
    if(strcmp(input,"\n") != 0){
      input[strlen(input)-1] = '\0';
      //scanf("%s", input);
      if(strcmp(input, "exit")==0){
        control = 0;
      }else if(input[strlen(input)-1] == '$'){
        int status = 0;
        pid_t part = getpid();
        fork();
          //partent
          waitpid(part, &status, 0);
          input[strlen(input)-1] = '\0';
          run_command_line(input, log_file, log_file_name);
      
      }else if(strlen(input) != 1){
        run_command_line(input, log_file, log_file_name);
      }
    }
    free(input);
    input = NULL;
    //if(!control)

  }
  
  if(log_file_name != NULL){
    Log_save(log_file, log_file_name); 
  }
  free_all(&command_log, &log_file, &command_file, &log_file_name);
  return 0;
}

void free_all(Log** command_log, Log** log_file, char** command_file, char** log_file_name){

  if(*command_log != NULL){
    printf("1\n");
    Log_destroy(*command_log);
  }
  if(*command_file != NULL){
    printf("2\n");
    free(*command_file);
  }
  if(*log_file_name != NULL){
    printf("3\n");
    free(*log_file_name);
  }
  if(*log_file != NULL){
    printf("4\n");
    Log_destroy(*log_file);
  }
  return;
}

void run_command_line(const char* command, Log* log, char* log_file){
  //printf("command: %s\n", command);
  //printf("strlen(command): %zu\n", strlen(command));
  //execl("/bin/ls/", "echo", "hello", NULL);
  //fix the exit later
  //
  //
  char org_command[strlen(command)+1];
  strcpy(org_command, command);
  if(strcmp(command, "exit")==0)
    return;
  size_t tok= 0;
  char** command_split = strsplit(command, " ", &tok);
  //printf("command_split[0]: %s\n", command_split[0]);
  //cd

  //printf("tok %zu\n",tok);
  if(strcmp(command_split[0],"cd") == 0){
    if(tok == 2){
      //command_split[1][strlen(command_split[1])-1] = '\0';
      //change working dir pwd
      //
      //
      if(chdir(command_split[1])==-1){
        print_no_directory(command_split[1]);
      }else{
        setenv("PWD", get_current_dir_name(), 1); 
      }
    }else{
      print_no_directory(*(command_split+1)); 
    }
  }else if(command_split[0][0] == '#'){
    //printf("we are doing #!!!!\n");
    command_split[0][0] = '0';
    size_t num = atoi(command_split[0]);
    //check if this is a number
    //
    //
    //printf("num: %zu\n", num);
    size_t log_size = Log_size(log);
    if(log_size < num){
      print_invalid_index();
    }else{
      run_command_line(Log_get_command(log, num), log, log_file);
    }
  }else if(command_split[0][0] == '!'){
    if(strcmp(command_split[0], "!history") == 0){
      for(size_t i = 0; i < Log_size(log); i++){
        printf("%zu\t%s\n", i, Log_get_command(log, i));
      }
    }else{
      int ran_command = 0;
      for(size_t i = Log_size(log); i > 0; i--){
        int tracker = 1;
        char hist_com[strlen(Log_get_command(log, i-1))];
        strcpy(hist_com, Log_get_command(log,i-1));
        for(size_t j = 1; j < strlen(command_split[0]); j++){
          if(command_split[0][j] != hist_com[j-1]){
            tracker = 0;
          }
          if(tracker){
            ran_command = 1;
            run_command_line(hist_com, log, log_file);
            break;
          }
        }
        if(tracker){
          break;
        }
      }
      if(!ran_command){
        print_no_history_match(); 
      }
    }
  }else{
    //everything else....
    int status = 0;
    pid_t pid = fork();
    //printf("printf command in else %s\n", command);
    size_t tok = 0;
    char** arg = strsplit(command, " ", &tok);
    if(pid == 0){
    //child
      //printf("arg char %c\n", *arg[tok-1]);
      //if(*arg[tok-1] == '$'){ 
      ////  exit(3);
      //  printf("wtf\n");
      //  Log* temp = Log_create();
      //  Log_save(temp, "temp.txt");
      //  char command[] = "./shell -h temp.txt";
      //  size_t temp_tok = 0;
      //  char** arg_command = strsplit(command, " ", &temp_tok);
      //  execvp(arg_command[0], arg_command);
      //}else{
        print_command_executed(getpid());
        if((execvp(arg[0], arg)==-1)){
          // failed
          free_args(arg); 
          //printf("wtf is going on\n");
          exit(3);
        }
      //}
      free_args(arg); 
      //printf("print last char %c\n", *arg[tok-1]);
     
      
    }else if(pid > 0){
      //partent
      //printf("command last letter %c\n", command[strlen(command)-1]);
      //if(command[strlen(command)-1] != '$'){
        waitpid(pid, &status, 0);
      
        if(WIFEXITED(status)){
          if(!WEXITSTATUS(status)){
          //everyone is okay 
          }else{
            print_exec_failed(command);
          }
        }else{
          print_exec_failed(command);
        }
      //}else{
      //  printf("do something here\n");
      //}
    }else{
      //fork failed
      print_fork_failed();
    }
    free_args(arg);
  }
  if(org_command[0] != '#' || org_command[0] != '!')
    Log_add_command(log, org_command);
  //free command_split

  free_args(command_split); 
}

