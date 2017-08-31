/**
 * Parallel Make
 * CS 241 - Spring 2016
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stddef.h>

/**
 * Queue Data Structure
 */
typedef struct {
  struct queue_node *head; ///<Head of linked-list
  struct queue_node *tail; ///<Tail of linked-list
  size_t size;             ///<Number of nodes in linked-list
} queue_t;

void queue_init(queue_t *q);
void queue_destroy(queue_t *q);

void *queue_dequeue(queue_t *q);
void *queue_at(queue_t *q, size_t pos);
void *queue_remove_at(queue_t *q, size_t pos);
void queue_enqueue(queue_t *q, void *item);
size_t queue_size(queue_t *q);

void queue_iterate(queue_t *q, void (*iter_func)(void *, void *), void *arg);

#endif
