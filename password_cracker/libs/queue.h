/**
 * Machine Problem: Password Cracker
 * CS 241 - Spring 2016
 */

#ifndef CRACKER_QUEUE_H
#define CRACKER_QUEUE_H

#include <pthread.h>

// implementation of a synchronized queue
typedef struct QueueNode {
  struct QueueNode *next;
  void *data;
} QueueNode;

typedef struct {
  pthread_mutex_t guard;
  pthread_cond_t signaler;
  QueueNode *head, *tail;
  int size, max_size;
} Queue;

void Queue_init(Queue *q, int max_size);
void Queue_destroy(Queue *q);

// Add a node to the queue. Always completes quickly without blocking.
void Queue_push(Queue *q, void *data);

// Remove a node from the queue. If the queue is empty, this will block
// until a node is added. If the queue has been shut down, returns NULL.
void *Queue_pull(Queue *q);

#endif /* CRACKER_QUEUE_H */
