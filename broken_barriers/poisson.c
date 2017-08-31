/**
 * Parallelized Poisson Problems
 * CS 241 - Spring 2016
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#ifndef _BMP_H
#define _BMP_H
#include "bmp.h"
#endif
#ifndef _BARRIER_H
#define _BARRIER_H
#include "barrier.h"
#endif
#ifndef _POISSON_H
#define _POISSON_H
#include "poisson.h"
#endif

static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
static double error;
static barrier_t bar;

// This helper function is provided modify current and previous.
// Use this function to split the work between threads.
// This funcition will update all things in the rectangle
// with startx and starty as its top-right point and
// width and height as the lengths of the respective sides.
// This function returns the error over the section
// of the arrays it was asked to modify.
double solve_poisson_helper(poisson_struct *info, int startx, int starty,
                            int width, int height) {
  double **previous = info->previous;
  double **target_image = info->target_image;
  double **current = info->current;

  int x, y;
  double error = 0;
  //int y = info->height;
  for (y = starty; y < height + starty; y+=info->num_threads) {
    for (x = startx; x < width + startx; x++) {
  //printf("before for loop %d %d\n", y, x);
      current[y][x] =
          0.25 * (previous[y][x + 1] + previous[y][x - 1] + previous[y + 1][x] +
                  previous[y - 1][x] - target_image[y][x]);
  //printf("after current\n");
      error += fabs(current[y][x] - previous[y][x]) *
               fabs(current[y][x] - previous[y][x]);
    }
  }
    //printf("barrier after first for loop\n");
    //barrier_wait(&bar);
    //printf("barrier_wait after\n");
  return error;
}

// This is the function we suggest having each
// indiviudal thread run. Inside of it we call
// the helper function, solve_poisson_helper,
// with the apropriate parameters.
void *solve_poisson(void *info_v) {
  poisson_struct *info = (poisson_struct *)info_v;
  int GRANULARITY = info->granularity;
  double EPSILON = info->epsilon;
  int N = info->n;
  //int NUM_THREADS = info->num_threads;
  

  info->iter = 0;
  double temp_error;
  do {
    barrier_wait(&bar);
    pthread_mutex_lock(&m);
    error = 0;
    pthread_mutex_unlock(&m);
    //printf("before solve %d\n", info->height);
    temp_error = solve_poisson_helper(info, 1, info->height, N, N);
    //printf("after solve %d\n", info->height);
    barrier_wait(&bar);
    pthread_mutex_lock(&m);
    error += temp_error;
    pthread_mutex_unlock(&m);
    //printf("before barrier_wait\n"); 

    barrier_wait(&bar);
    //printf("after barrier_wait\n"); 
    info->iter++;
    if (info->iter % GRANULARITY == 0){
      if(info->height == 1)
        write_to_file(info->current, 0, 0, N + 2, N + 2, info->iter);
    }
    double **temp = info->previous;
    info->previous = info->current;
    info->current = temp;
    //printf("before 2nd barrier\n");
    //barrier_wait(&bar);
    //printf("after 2nd barrier\n");
  } while (error > EPSILON);

  //for loop to add errors

  double **temp = info->previous;
  info->previous = info->current;
  info->current = temp;
  if(info->height == 1)
    write_to_file(info->current, 0, 0, N + 2, N + 2, info->iter);
  barrier_wait(&bar);
  return info;
}

// Sets up the poisson_struct so it has a 2-d array of pointers for the old
// iteration and the new iteration. Returns arrays of size n+2
// to handle the edge conditions
poisson_struct **poisson_setup(int n, double **target_image, int num_threads,
                               int granularity, double epsilon) {
  
  int n_temp = n;
  n += 2;
  int i;
  poisson_struct **ps_array = malloc(sizeof(poisson_struct *) * num_threads);
  double *previous = calloc(sizeof(double), n * n);
  double *current = calloc(sizeof(double), n * n);

  double **previous_ret = malloc(sizeof(double *) * n);
  double **current_ret = malloc(sizeof(double *) * n);

  int actual = 0;
  // This is the abstraction of a 1-D array of memory into a 2-D array
  for (i = 0; i < n; i++, actual += n) {
    previous_ret[i] = &previous[actual];
    current_ret[i] = &current[actual];
  }

  //split image
  int temp = 1;

  for (i = 0; i < num_threads; i++) {
    ps_array[i] = malloc(sizeof(poisson_struct));
    ps_array[i]->n = n_temp;
    ps_array[i]->num_threads = num_threads;
    ps_array[i]->granularity = granularity;
    ps_array[i]->epsilon = epsilon;
    ps_array[i]->previous = previous_ret;
    ps_array[i]->current = current_ret;
    ps_array[i]->target_image = target_image;
    ps_array[i]->iter = 0;
    ps_array[i]->height = temp + i;
  }
  return ps_array;
}

// Cleans up the abstraction of a 1-d array
// into a 2-d array that we created in
// poisson_setup
void poisson_destroy(poisson_struct **ps_array, int num_threads) {
  poisson_struct *ps = *ps_array;
  free(*(ps->current));
  free(*(ps->previous));
  free(ps->current);
  free(ps->previous);
  int i;
  for (i = 0; i < num_threads; i++)
    free(ps_array[i]);
  free(ps_array);
}

int serial_poisson(int n, int granularity, double epsilon,
                   double **target_image) {
  // All the arrays inside the poisson_struct array will be initialized with
  // their buffer sizes to (N+2) x (N+2). It is up to the user to make sure
  // that target_image is sufficiently large  (i.e. at least (n+2)x(n+2))
  poisson_struct **ps_array =
      poisson_setup(n, target_image, 1, granularity, epsilon);

  solve_poisson(ps_array[0]);
  int iter_ret = ps_array[0]->iter;
  poisson_destroy(ps_array, 1);
  return iter_ret;
}

// This is the function that we will be using for testing. The ouput should be
// identical
// to that of serial_poisson.
int parallel_poisson(int n, int granularity, double epsilon,
                     double **target_image, int num_threads) {
 
  //truncating num_threads
  num_threads = num_threads%(n+1);
  //if(num_threads > 8)
  //  num_threads = 8;
  
  pthread_t thread[num_threads];
  poisson_struct** ps_array = poisson_setup(n, target_image, num_threads, granularity, epsilon);
  barrier_init(&bar, num_threads);
  
  int inter_ret = 0;
  for(int i = 0; i < num_threads; i++){
    pthread_create(&thread[i], NULL, (void*)solve_poisson, ps_array[i]);
  }
  for(int i = 0; i < num_threads; i++){
    pthread_join(thread[i], NULL);
    inter_ret += ps_array[i]->iter;
  }
  poisson_destroy(ps_array, num_threads);
  barrier_destroy(&bar);
  return inter_ret;
}
