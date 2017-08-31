/**
 * Parallelized Poisson Problems
 * CS 241 - Spring 2016
 */
#include "barrier.h"
#include <stdio.h>
// The returns are just for errors if you want to check for them.
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


//    while(t->count != 0){
//      pthread_cond_wait(&t->cv, &t->mtx);
//    }
//printf("after while\n");
//    pthread_mutex_lock(&t->mtx);
//    t->count++;
//    unsigned int temp = t->count/t->n_threads;
//    t->times_used = temp;
//    if(t->count%t->n_threads == 0){
//      pthread_cond_broadcast(&t->cv);
//    }else{
//      while(t->times_used == temp){
//        pthread_cond_wait(&t->cv, &t->mtx);
//      }
//    }
//    t->count--;
//    if(t->count == 0){
//      pthread_cond_broadcast(&t->cv);
//    }
//    pthread_mutex_unlock(&t->mtx);
//  return 0; 
}
