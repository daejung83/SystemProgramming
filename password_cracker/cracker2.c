/**
 * Machine Problem: Password Cracker
 * CS 241 - Spring 2016
 */

#include "cracker2.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue.h>
#include <crypt.h>
#include <semaphore.h>
#include <stdbool.h>
#include "libs/format.h"
#include "libs/utils.h"

typedef struct barrier_t {
  pthread_mutex_t mtx;
  pthread_cond_t cv;
  // Total number of threads
  unsigned n_threads;
  // Increasing or decreasing count
  unsigned count;
  // Keeps track of what usage number of the barrier we are at
  unsigned int times_used;
} barrier_t;

static Queue* t;
static barrier_t* barr;
static pthread_mutex_t m;
static pthread_cond_t cv;
static int gar = -123;
static size_t thread_num;
static int pass_found;
static char* final_password;
static int total_hash;

char **strsplit(const char *str, const char *delim, size_t *numtokens);
void* create_data_struct(char* data);
void free_args(char **args);

typedef struct user_data{
  char* user_name;
  char* hash;
  char* given_pass;
  size_t crack_num;
} user_data;

static user_data* data_space;

int barrier_destroy(barrier_t *t) {
  int error = 0;
  pthread_mutex_destroy(&t->mtx);
  pthread_cond_destroy(&t->cv);
  return error;
}

int barrier_init(barrier_t *t, unsigned n) {
  int error = 0;
  pthread_mutex_init(&t->mtx, NULL);
  pthread_cond_init(&t->cv, NULL);
  t->n_threads = n;
  t->count = 0;
  return error;
}

int barrier_wait(barrier_t *t) { 

//  printf("barrier_wait started\n");
  
  pthread_mutex_lock(&t->mtx);
  t->count++;
  unsigned int temp = t->count/t->n_threads;
  if(t->count%t->n_threads == 0){
    pthread_cond_broadcast(&t->cv);
  }else{
    while(t->count/t->n_threads == temp){
      pthread_cond_wait(&t->cv, &t->mtx);
    }
  }
  pthread_mutex_unlock(&t->mtx);

//  printf("barrier_wait finished\n");
  return 0; 
}

void* create_data_struct(char* data){
  user_data* user = malloc(sizeof(user_data));
  size_t tok;
  char* del = " ";
  char** elems = strsplit(data, del, &tok);
  user->user_name = strdup(elems[0]);
  user->hash = strdup(elems[1]);
  user->given_pass = strdup(elems[2]);
  user->crack_num = 0;
  size_t len = strlen(elems[2]);
  //printf("%s\n", user->given_pass);
  for(size_t i = 0; i < len; i++){
    if(user->given_pass[i] == '.'){
      user->crack_num++;
      user->given_pass[i] = '\0';
    }
  }
  free_args(elems);
  //printf("%zu %s %zu\n", len, user->given_pass, user->crack_num);
  return (void*)user;
}

char **strsplit(const char *str, const char *delim, size_t *numtokens) {
  // copy the original string so that we don't overwrite parts of it
  // (don't do this if you don't need to keep the old line,
  // as this is less efficient)
  char *s = strdup(str);
  // these three variables are part of a very common idiom to
  // implement a dynamically-growing array
  size_t tokens_alloc = 1;
  size_t tokens_used = 0;
  char **tokens = calloc(tokens_alloc, sizeof(char *));
  char *token, *strtok_ctx;
  for (token = strtok_r(s, delim, &strtok_ctx); token != NULL;
       token = strtok_r(NULL, delim, &strtok_ctx)) {
    // check if we need to allocate more space for tokens
    if (tokens_used == tokens_alloc) {
      tokens_alloc *= 2;
      tokens = realloc(tokens, tokens_alloc * sizeof(char *));
    }
    tokens[tokens_used++] = strdup(token);
  }
  // cleanup
  if (tokens_used == 0) {
    free(tokens);
    tokens = NULL;
  } else {
    tokens = realloc(tokens, tokens_used * sizeof(char *));
  }
  *numtokens = tokens_used;
  free(s);
  // Adding a null terminator
  tokens = realloc(tokens, sizeof(char *) * (tokens_used + 1));
  tokens[tokens_used] = NULL;
  return tokens;
}

void free_args(char **args) {
  char **ptr = args;
  while (*ptr) {
    free(*ptr);
    ptr++;
  }
  free(args);
}

void* get_data(){
  char* buffer = NULL;
  size_t buffer_size;
  ssize_t size;
  while((size = getline(&buffer, &buffer_size, stdin)) != -1){
    //printf("%s %d\n", buffer, (int)size);

    void* temp = create_data_struct(buffer);
    pthread_mutex_lock(&m);
    //while(t->max_size == t->size)
    //  pthread_cond_wait(&cv, &m);
    Queue_push(t, temp);
    //Queue_push(t, create_data_struct(buffer));

    //pthread_cond_signal(&cv);
    pthread_mutex_unlock(&m);
  }
  //pthread_cond_broadcast(&cv);
  free(buffer);
  pthread_mutex_lock(&m);
  Queue_push(t, (void*)&gar);
  pthread_cond_broadcast(&cv);
  pthread_mutex_unlock(&m);
  //input_sig = 0;
  return NULL; 
}

void* crack_password(void* thread_id){
	struct crypt_data cdata;
	cdata.initialized = 0;
	int start_time = 0;
  int start_getTime = 0;
	while(1){
    barrier_wait(barr);
    if(*((int*)thread_id) == 1){
    final_password = NULL;
		pass_found = 0;
		total_hash = 0;
			start_time = getCPUTime();
      start_getTime = getTime();
		  pthread_mutex_lock(&m);
	  	while(t->size == 0){
			  pthread_cond_wait(&cv, &m);
		  }
		  data_space = Queue_pull(t);
		  pthread_mutex_unlock(&m);
			if((int*)data_space != &gar){
				v2_print_start_user(data_space->user_name);
			}
    }
    barrier_wait(barr);
		if((int*)data_space == &gar){
    	Queue_push(t, (void*)&gar);
    	pthread_cond_signal(&cv);
    	return NULL;
		}

    long start_index, count;
    getSubrange(data_space->crack_num, thread_num, *((int*)thread_id), &start_index, &count);
		//char* temp_pass = strdup(data_space->given_pass);
		//printf("this is temp_pass %s\n", temp_pass);
		//printf("start_index %lu, count %lu\n", start_index, count);
		char* temp_pass = malloc(data_space->crack_num + 1);
    for(size_t i = 0; i < data_space->crack_num; i++){
      temp_pass[i] = 'a';
    }
    temp_pass[data_space->crack_num] = '\0';
		setStringPosition(temp_pass, start_index);
    char* password = malloc(strlen(data_space->given_pass) + data_space->crack_num + 1);
		strcpy(password, data_space->given_pass);
		strcat(password, temp_pass);
		password[strlen(data_space->given_pass) + data_space->crack_num] = '\0';
		//print start
		v2_print_thread_start(*((int*)thread_id), data_space->user_name, start_index, password);
		int result_sig = 2;
		int hash_count = 0;
		for(long i = 0; i < count; i++){
			hash_count++;
			char* result = crypt_r(password, "xx", &cdata);
			if(0 == strcmp(result, data_space->hash)){
				pthread_mutex_lock(&m);
				total_hash += hash_count;
				pass_found = 1;
				pthread_mutex_unlock(&m);
        final_password = strdup(password);
				//final_password = password;
				result_sig = 0;
				break;
			}else{
				incrementString(password);	
			}
			pthread_mutex_lock(&m);
			if(pass_found){
				total_hash += hash_count;
				pthread_mutex_unlock(&m);
				result_sig = 1;
				break;
			}
			pthread_mutex_unlock(&m);
		}
    pthread_mutex_lock(&m);
    if(!pass_found){
      total_hash += hash_count;  
    }
		v2_print_thread_result(*((int*)thread_id), hash_count, result_sig);
    pthread_mutex_unlock(&m);
		//barrier wait
		barrier_wait(barr);
		//need total hash
		if(*((int*)thread_id) == 1){
			v2_print_summary(data_space->user_name, final_password, total_hash, getTime() - start_getTime, getCPUTime() - start_time, !pass_found);
    }
    barrier_wait(barr);
    if(*((int*)thread_id) == 1 && data_space != NULL){
      free(data_space->user_name);
			free(data_space->given_pass);
			free(data_space->hash);
      free(data_space);
      data_space = NULL;
      if(final_password != NULL)
        free(final_password);
    }
    free(password);
		free(temp_pass);
	}	
}

int start(size_t thread_count) {
  // TODO your code here, make sure to use thread_count!
	thread_num = thread_count;
	pthread_t threads[thread_count];
	pthread_mutex_init(&m, NULL);
	pthread_cond_init(&cv, NULL);
	t = malloc(sizeof(Queue));
	barr = malloc(sizeof(barrier_t));
	Queue_init(t, 100);
	barrier_init(barr, thread_count);
  get_data();
	int thread_id[thread_count + 1];
	for(int i = 0; i < (int)thread_count; i++){
		thread_id[i] = i+1;
		pthread_create(&threads[i], NULL, crack_password, (void*)&thread_id[i]);
	}	
	for(size_t i = 0; i < thread_count; i++){
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&m);
	pthread_cond_destroy(&cv);
	Queue_destroy(t);
	barrier_destroy(barr);
	free(t);
	free(barr);
  return 0;
}
