/**
 * Mangled Mutexes Lab
 * CS 241 - Spring 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

/* You should create a struct that will get passed in by reference to your
 * start_routine. */

// Globals
size_t number_of_primes; // global number of primes you need to update
/* You should create a mutex for said global variable. */
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
/* You should create a start routine for your threads. */

// This function parses the arguments in argv.
// An error message is printed if any of the following happens:
//  An incorrect number of arguments are passed in.
//  'start', 'end', or 'num_threads' is less than 1.
//  'end' < 'start'.
// Else an array of 3 size_t elements are returned
// in the following order: 'start', 'end', 'num_threads'.
// It is the callers job to free the memory used.
typedef struct passing_arg{
  size_t arg1;
  size_t arg2;
}Arg;
size_t *arg_parse(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(stderr, "usage: %s [start] [end] [num_threads]\n", argv[0]);
    exit(1);
  }

  char *endptr;
  long tokargs[3];
  for (size_t i = 0; i < sizeof(tokargs) / sizeof(tokargs[0]); i++) {
    tokargs[i] = strtol(argv[i + 1], &endptr, 10);
    if (*endptr != '\0') {
      fprintf(stderr, "Failed to convert an arugment to a long!\n");
      exit(2);
    }
    if (tokargs[i] < 1) {
      fprintf(stderr,
              "Please have all arguments be greater than or equal to 1!\n");
      exit(3);
    }
  }
  if (tokargs[1] < tokargs[0]) {
    fprintf(stderr, "Please give a valid range!\n");
    exit(4);
  }
  size_t *args = malloc(3 * sizeof(size_t));
  for (size_t i = 0; i < sizeof(tokargs) / sizeof(tokargs[0]); i++) {
    args[i] = (size_t)tokargs[i];
  }
  return args;
}

// Call on this function when you are ready to print the result.
void print_result(size_t number_of_primes, size_t start, size_t end) {
  printf("There are %zu primes between %zu and %zu inclusive\n",
         number_of_primes, start, end);
}
void check_prime_norm(size_t start, size_t end){

  size_t num_sqrt;
    for(size_t i = start; i < end+1; i=i+2){
      num_sqrt = ceil(sqrt(i))+1;
      //printf("%zu ", num_sqrt);
      if(num_sqrt > 1){
        int condition = 2;
        //printf("num_sqrt: %zu\n", num_sqrt);
        for(size_t j = 3; j < num_sqrt; j++){
          condition = 1;
          if((i%j) == 0){
            condition = 0;
            break;
          }
        }
        if(condition == 1){
          //printf("%zu ", i);
          //p_count++;
          number_of_primes++;
        }
      }
    }

}
//void check_prime(size_t start, size_t end){
void* check_prime(void* arg){
  
  size_t start = ((Arg*)arg)->arg1;
  size_t end = ((Arg*)arg)->arg2;
  size_t num_sqrt;
  if(start%2 == 0){
    start++;
  }
  
    //printf("start: %zu end: %zu\n", start, end);
    for(size_t i = start; i < end+1; i=i+2){
      num_sqrt = ceil(sqrt(i))+1;
      //printf("%zu ", num_sqrt);
      if(num_sqrt > 1){
        int condition = 2;
        //printf("num_sqrt: %zu\n", num_sqrt);
        for(size_t j = 3; j < num_sqrt; j++){
          condition = 1;
          if((i%j) == 0){
            condition = 0;
            break;
          }
        }
        if(condition == 1){
          //printf("%zu ", i);
          //p_count++;
          pthread_mutex_lock(&m); 
          number_of_primes++;
          pthread_mutex_unlock(&m);
        }
      }
    }
  return NULL;
}
// Prints the number of primes
// inbetween argv[1] and argv[2] inclusive with argv[3] threads.
int main(int argc, char *argv[]) {
  /* Your implementation goes here */
  size_t* arg_num = arg_parse(argc, argv);
  size_t start = arg_num[0];
  size_t end = arg_num[1];
  size_t num_treads = arg_num[2];
  //printf("start: %zu\n", start);
  size_t s_e_diff = end - start;
  if(s_e_diff < num_treads){
    num_treads = s_e_diff;
  }
  
  if(start < 2 && end < 2){
   //do nothing 
  }else if(start < 3){
    //p_count = p_count + 2;
    pthread_mutex_lock(&m); 
    number_of_primes = number_of_primes + 2;
    pthread_mutex_unlock(&m);
  }else if(start < 4){
    //p_count++;
    pthread_mutex_lock(&m); 
    number_of_primes++;
    pthread_mutex_unlock(&m);
  }
  if(num_treads > 1){
    size_t tread_diff = ceil(s_e_diff/num_treads);
    //printf("tread_diff: %zu\n", tread_diff);

    pthread_t threads[num_treads];
  
    Arg arg;
    arg.arg1 = start;
    arg.arg2 = start+tread_diff;
    
    Arg arg_temp[num_treads];

    for(size_t i = 1; i < num_treads-1; i++){
    //  check_prime(start+(tread_diff*i)+1, start+(tread_diff*(i+1)));
      arg_temp[i].arg1 = start +(tread_diff*i)+1;
      arg_temp[i].arg2 = start +(tread_diff*(i+1));
    }
    
    pthread_create(&threads[0], NULL, check_prime, &arg);
    //check_prime(start, start+tread_diff);
    for(size_t i = 1; i < num_treads-1; i++){
    //  check_prime(start+(tread_diff*i)+1, start+(tread_diff*(i+1)));
      arg_temp[i].arg1 = start +(tread_diff*i)+1;
      arg_temp[i].arg2 = start +(tread_diff*(i+1));
    
      pthread_create(&threads[i], NULL, check_prime, &arg_temp[i]);
    }
    //check_prime(start+1+(tread_diff*(num_treads-2)), end); 

    Arg arg_last;
    arg_last.arg1 = start+1+(tread_diff*(num_treads-1));
    arg_last.arg2 = end;

    pthread_create(&threads[num_treads-1], NULL, check_prime, &arg_last);
    //pthread_exit(NULL);
    for(size_t i = 0; i < num_treads; i++){
      pthread_join(threads[i], NULL);
    }
  }else{
    check_prime_norm(start, end);
  }
    
  //printf("number_of_primes: %zu\n", number_of_primes);
  print_result(number_of_primes, start, end);

  return 0;
}


