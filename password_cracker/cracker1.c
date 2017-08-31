/**
 * Machine Problem: Password Cracker
 * CS 241 - Spring 2016
 */


#include "cracker1.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue.h>
#include <crypt.h>
#include "libs/format.h"
#include "libs/utils.h"

static Queue* t;
static int numRecovered;
static int numFailed;
//static int input_sig;
static double total_time;
//static double total_cpu_time;
static int gar = -123;
static pthread_mutex_t m;
static pthread_cond_t cv;

char **strsplit(const char *str, const char *delim, size_t *numtokens);
void* create_data_struct(char* data);
void free_args(char **args);

typedef struct user_data{
  char* user_name;
  char* hash;
  char* given_pass;
  size_t crack_num;
} user_data;

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

void* crack_password(void* num){

  while(1){
  pthread_mutex_lock(&m);
  while(t->size == 0){
    pthread_cond_wait(&cv, &m);
  }
  user_data* data = Queue_pull(t);
  if((int*)data == &gar){
    Queue_push(t, (void*)&gar);
    pthread_cond_signal(&cv);
    //printf("testing\n");
    pthread_mutex_unlock(&m);
    return NULL;
  }
  pthread_mutex_unlock(&m);
  v1_print_thread_start(*((int*)num), data->user_name);
  double start = getTime();
  char* abc = "abcdefghijklmnopqrstuvwxyz";

  struct crypt_data cdata;
  cdata.initialized = 0;
  //sample
  //char* test = crypt_r(data->given_pass, "xx", &cdata);
  //printf("hash: %s\n", test);

  char passcode[data->crack_num];
  int pass_index[data->crack_num];
  for(size_t i = 0; i < data->crack_num; i++){
    passcode[i] = abc[0];
    pass_index[i] = 0;
  }
  char password[strlen(data->given_pass) + data->crack_num + 1];
  size_t hash_count = 0;
  int result_control = 1;
  while(1){
    hash_count++;
    strcpy(password, data->given_pass);
    strcat(password, passcode);
    password[strlen(data->given_pass) + data->crack_num] = '\0';
    
    char* result = crypt_r(password, "xx", &cdata);
    if(0==strcmp(result, data->hash)){
      //found same
      //printf("password: %s %zu\n", password, strlen(password));
      pthread_mutex_lock(&m);
      numRecovered++;
      pthread_mutex_unlock(&m);
      result_control = 0;
      break;
    }
    //free(result);
    for(size_t j = data->crack_num; j > 0; j--){
      if(j == data->crack_num){
        pass_index[j-1]++;
      }else{
        if(pass_index[j] == 26){
          pass_index[j-1]++;
          pass_index[j] = 0;
        }
      }
    }
    if(pass_index[0] == 26){
      break;
    }
    for(size_t k = 0; k < data->crack_num; k++){
      passcode[k] = abc[pass_index[k]];
    }
  }
  if(result_control){
    pthread_mutex_lock(&m);
    numFailed++;
    pthread_mutex_unlock(&m);
  }
  double time = getTime() - start;
  v1_print_thread_result(*((int*)num), data->user_name, password, hash_count, time, result_control);
  if(time > total_time)
    total_time = time;

  free(data->user_name);
  data->user_name = NULL;
  free(data->hash);
  data->hash = NULL;
  free(data->given_pass);
  data->given_pass = NULL;
  free(data);
  data = NULL;
  
  }
  return NULL;
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

int start(size_t thread_count) {
  //input_sig = 1;
  // your code here
  // make sure to make thread_count threads
  //double start_t = getTime();
  pthread_t threads[thread_count+1];
  double start_time = getCPUTime();
  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&cv, NULL);
  t = malloc(sizeof(Queue));

  Queue_init(t, 100);

  int thread_id[thread_count + 1];

  //while(t->size != 0){
    for(int i = 0; i <= (int)thread_count; i++){
      //crack_password((void*)&i); 
      if(i == 0){
        pthread_create(&threads[0], NULL, get_data, NULL);
      }else{
        thread_id[i] = i;
        pthread_create(&threads[i], NULL, crack_password, (void*)&thread_id[i]);
      }
    }
  //}
  pthread_cond_broadcast(&cv);
  for(size_t i = 0; i <= thread_count; i++ ){
    pthread_join(threads[i], NULL);
  }
  //join threads
  //v1_print_summary
  v1_print_summary(numRecovered, numFailed, total_time, getCPUTime() - start_time);
  
  pthread_mutex_destroy(&m);
  pthread_cond_destroy(&cv);
  Queue_destroy(t); 
  free(t);
  return 0;
}
