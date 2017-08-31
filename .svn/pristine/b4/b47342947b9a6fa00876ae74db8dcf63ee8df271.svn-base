/**
 * Parallel Make
 * CS 241 - Spring 2016
 */

#include "parmake.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "queue.h"
#include "parser.h"
#include "rule.h"
#include <pthread.h>

static queue_t* q;
static queue_t* q_status;
static pthread_mutex_t m;
static pthread_cond_t cv;
static int dummy = -123;
static size_t count;
//static int THE_KILL_SIG;
//static queue_t* q_depen;
//static queue_t* q_command;

typedef struct finished_status{
  char* target;
  int done_status;
}finished_status;

typedef struct chars{
  char* target_char;
  char* target_option;
}chars;

typedef struct com_temp{
  int* sig;
  char* val;
}com_temp;

void iter_depen(void* target, void* depen){
  if(0 == strcmp(((rule_t*)target)->target, ((chars*)depen)->target_char)){
    queue_enqueue(((rule_t*)target)->deps, strdup(((chars*)depen)->target_option));
  }
}

void iter_command(void* target, void* command){
  if(0 == strcmp(((rule_t*)target)->target, ((chars*)command)->target_char)){
    queue_enqueue(((rule_t*)target)->commands, strdup(((chars*)command)->target_option));
  }
}

void parsed_new_target(char* target){
	//printf("parsed_new_target:target: |%s|\n", target);  
  rule_t* rule = malloc(sizeof(rule_t));
  rule_init(rule);
  rule->target = strdup(target);
  finished_status* fs = malloc(sizeof(finished_status));
  fs->target = strdup(target);
  fs->done_status = 0;
  //queue_enqueue(rule->deps, (void*)&dummy);
  //queue_enqueue(rule->commands, (void*)&dummy);
  queue_enqueue(q, rule);
  queue_enqueue(q_status, fs);
}

void parsed_new_dependency(char* target, char* dependency){
	//printf("parsed_new_dependency:target: |%s| dependency |%s|\n", target, dependency);
  chars arg;
  arg.target_char = target;
  arg.target_option = dependency;
  queue_iterate(q, iter_depen, (void*)&arg);
}

void parsed_new_command(char* target, char* command){
  //printf("parsed_new_command:target: |%s| command: |%s|\n", target, command);
  chars arg;
  arg.target_char = target;
  arg.target_option = command;
  queue_iterate(q, iter_command, (void*)&arg);
}

void check_queue(void* item, void* arg){
  com_temp* args = arg;
  finished_status* rule = item;
  //printf("args->val: %s\n", args->val);
  //printf("args->val: %s, item: %s,\n", args->val, (char*)item);
  if(0 == strcmp(args->val, rule->target)){
    //printf("they are same!\n");
    if(rule->done_status == 0){
      *(args->sig) = 0;
    }
  }
}
void mark_used(void* item, void* arg){
  com_temp* args = arg;
  finished_status* rule = item;

  if(0 == strcmp(args->val, rule->target)){
    //printf("changed: %s\n", args->val);
    rule->done_status = 1;
  }
}

void* run_commands(){
  pthread_mutex_lock(&m);
  while(queue_size(q) < 1){
    pthread_cond_wait(&cv, &m);
  }
  rule_t* data = queue_dequeue(q);
  pthread_mutex_unlock(&m);
  int can_use = 1;
  com_temp arg;
  arg.sig = &can_use;
  while(1){
    can_use = 1;
    //printf("count: %lu\n", count);
    //pthread_mutex_lock(&m);
    //printf("kill %d\n", THE_KILL_SIG);
    //if(THE_KILL_SIG){
    //  queue_enqueue(q, data);
    //  pthread_mutex_unlock(&m);
    //  return NULL;
    //}
    //pthread_mutex_unlock(&m);
    pthread_mutex_lock(&m);
    if(data == (void*) &dummy && count == 0){
      queue_enqueue(q, data);
      pthread_cond_signal(&cv);
      pthread_mutex_unlock(&m);
      return NULL;
    }else if(data == (void*)&dummy){
      queue_enqueue(q, data);
    }
    pthread_mutex_unlock(&m);
    //printf("data->target: %s\n", data->target);
    if(data != (void*)&dummy){
      pthread_mutex_lock(&m); 
      for(size_t i = 0; i < queue_size(data->deps); i++){
        arg.val = queue_at(data->deps, i);
        //printf("arg.val: %s\n", arg.val);
        queue_iterate(q_status, check_queue, (void*)&arg);
      }
      pthread_mutex_unlock(&m);
      //printf("can_use: %d\n", can_use);
      if(can_use){
        char* final_command = queue_dequeue(data->commands);
        while(final_command != NULL){
          if(system(final_command) != 0){
            //printf("final_command\n");
            //pthread_mutex_lock(&m);
            //THE_KILL_SIG = 1;
            //queue_enqueue(q, data);
            //pthread_mutex_unlock(&m);
            //return NULL;
            break;
          }
          final_command = queue_dequeue(data->commands);
        }
        pthread_mutex_lock(&m);
        count--;
        arg.val = data->target;
        queue_iterate(q_status, mark_used, (void*)&arg);
        pthread_mutex_unlock(&m);
      }else{
        //printf("can use %d\n", can_use);
        pthread_mutex_lock(&m);
        queue_enqueue(q, data);
        //pthread_cond_signal(&cv);
        pthread_mutex_unlock(&m);
      }

    }
    pthread_mutex_lock(&m);
    while(queue_size(q) < 1){
      pthread_cond_wait(&cv, &m);
    }
    data = queue_dequeue(q);
    pthread_mutex_unlock(&m);
  }
  return NULL;
}

// Treat this as main
int parmake(int argc, char **argv) {
  // good luck!
  if(argc < 3){
		fprintf(stderr, "./path/to/parmake [-f &lt;path/to/makefile&gt;] [-j &lt;positive-integer&gt;] [targets ...]\n");
    return 1;
  }
	char* fvalue = NULL;
	//char* jvalue = NULL;
  int num_threads = 0;
	opterr = 0;
	int fcount = 0;
	int jcount = 0;
	int c;
  //THE_KILL_SIG = 0;
  
	while((c = getopt(argc, argv, "f:j:")) != -1){
		switch(c){
			case 'f':
				fvalue = optarg;
				fcount++;
				break;
			case 'j':
        for(size_t i = 0; i < strlen(optarg); i++){
          if(!isdigit(optarg[i])){
            fprintf(stderr, "%s is not a number\n", optarg);
            return 1;
          }
        }
				num_threads = atoi(optarg);
				jcount++;
				break;
			default:
				fprintf(stderr, "./path/to/parmake [-f &lt;path/to/makefile&gt;] [-j &lt;positive-integer&gt;] [targets ...]\n");
				return 1;
				break;	
		}
	}
	if(fcount + jcount > 2){
		fprintf(stderr, "to many arguments\n");
		return 1;
	}	
	//printf("fvalue: %s, jvalue: %s\n", fvalue, jvalue);
  //int num_threads = atoi(jvalue);
  //printf("argv: %s\n", argv[5]);
  //printf("argc: %d\n", argc);
  char** targets = malloc(sizeof(void*) * (argc - 5)); 
  for(int i = 5; i < argc; i++){
    targets[i-5] = argv[i];
  }
  //printf("fvalue: %s\n", fvalue);
  if(num_threads == 0){
    num_threads = 1;
  }
  q = malloc(sizeof(queue_t));
  q_status = malloc(sizeof(queue_t));
	queue_init(q);
  queue_init(q_status);
  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&cv, NULL);

  parser_parse_makefile(fvalue, targets, parsed_new_target, parsed_new_dependency, parsed_new_command); 
  queue_enqueue(q, &dummy);
  //printf("start parser\n");	
  //rule_t* data = queue_dequeue(q);
  //while(data != NULL){
  //  printf("target: %s\n", data->target);

  //  char* depend = queue_dequeue(data->deps);
  //  while(depend != NULL){
  //    printf("deps: %s\n", depend);
  //    depend = queue_dequeue(data->deps);
  //  }
  //  char* com = queue_dequeue(data->commands);
  //  while(com != NULL){
  //    printf("commands: %s\n", com);
  //    com = queue_dequeue(data->commands);
  //  }
  //  data = queue_dequeue(q);
  //}
  //everything is in the queue now
  count = queue_size(q) - 1;
  pthread_t threads[num_threads];
  for(int i = 0; i < num_threads; i++){
    pthread_create(&threads[i], NULL, run_commands, NULL);
  }
  //pthread_cond_broadcast(&cv);
  //run_commands(); 
  for(int i = 0; i < num_threads; i++){
    //printf("hello\n");
    pthread_join(threads[i], NULL);
  }
  queue_destroy(q_status);
	queue_destroy(q);
  pthread_mutex_destroy(&m);
  pthread_cond_destroy(&cv);
  free(q);
  free(q_status);
  free(targets);

  return 0;
}
