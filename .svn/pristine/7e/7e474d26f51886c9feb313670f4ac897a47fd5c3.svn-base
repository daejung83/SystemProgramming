/**
 * Terrible Threads Lab
 * CS 241 - Spring 2016
 */
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

static pthread_t tid1, tid2, tid3, tid4;

static int ip, ic; // producer consumer counts

static pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

#define OPS (10001)

long sum;
int *integers;

void *sum_producer() {
  usleep(rand() & 15); /// Random Start
  puts("Producer starting");

  int i;
  for (i = 1; i < OPS; i++) {
    pthread_mutex_lock(&m);
    integers[ip] = i;
    ip++;
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&m);
  }
  puts("producer quitting");
  return NULL;
}

void *sum_consumer() {
  usleep(rand() & 15); // Random Start
  puts("consumer starting");

  int i;
  for (i = 1; i < OPS; i++) {
    pthread_mutex_lock(&m);
    while (ic >= ip)
      pthread_cond_wait(&cv, &m);
    sum += integers[ic];
    ic++;
    pthread_mutex_unlock(&m);
  }
  puts("consumer quitting");
  return NULL;
}

// Test that sums up integers in an array
int sum_array_test() {
  puts("Beginning Sum Test:");
  int err = pthread_create(&tid1, 0, sum_producer, NULL);
  int err2 = pthread_create(&tid2, 0, sum_producer, NULL);
  int err3 = pthread_create(&tid3, 0, sum_consumer, NULL);
  int err4 = pthread_create(&tid4, 0, sum_consumer, NULL);
  if (err || err2 || err3 || err4)
    printf("BIG UHOH\n");
  puts("Waiting on producer 1...");
  pthread_join(tid1, NULL);
  puts("Waiting on producer 2...");
  pthread_join(tid2, NULL);
  puts("Waiting on consumer 1...");
  pthread_join(tid3, NULL);
  puts("Waiting on consumer 2...");
  pthread_join(tid4, NULL);
  printf("Total sum %ld values\n", sum);
  free(integers);

  // Sum of integers = N (N+1) / 2
  // (and we have every integer 1..10000 twice)
  assert(sum == 100010000L);
  puts("Passed");
  return 0;
}

int main(int argc, char **argv) {

  integers = malloc(sizeof(int) * OPS * 2);
  sum_array_test();
  exit(1);
}
