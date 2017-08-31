/**
 * Terrible Threads Lab
 * CS 241 - Spring 2016
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

/**
 *  Given data, place it on the queue.  Can be called by multiple threads.
 */
void queue_push(queue_t *queue, void *data) { 
  /* Your code here */ 
  queue_node_t* n_node = malloc(sizeof(queue_node_t));
  n_node->data = data;
  n_node->next = NULL;
  pthread_mutex_lock(&queue->m); 
  while(queue->size >= queue->maxSize){
    pthread_cond_wait(&queue->cv, &queue->m);
  }
  if(queue->head != NULL){
    queue->tail->next = n_node;
    queue->tail = n_node;
  }else{
    queue->head = n_node;
    queue->tail = n_node;
  }
  queue->size++;
  pthread_cond_signal(&queue->cv);
  pthread_mutex_unlock(&queue->m);
}

/**
 *  Retrieve the data from the front of the queue.  Can be called by multiple
 * threads.
 *  Blocks the queue is empty
 */
void *queue_pull(queue_t *queue) {
  /* Your code here */

  queue_node_t* n_node = NULL;
  pthread_mutex_lock(&queue->m);
  while(queue->size <= 0){
    pthread_cond_wait(&queue->cv, &queue->m);
  }
  n_node = queue->head;
  queue->head = queue->head->next;
  queue->size--;
  void* result = n_node->data;
  free(n_node);
  pthread_cond_signal(&queue->cv);
  pthread_mutex_unlock(&queue->m);
  return result;
}

/**
 *  Initializes the queue
 */
void queue_init(queue_t *queue, int maxSize) { 
  /* Your code here */ 
  //queue = malloc(sizeof(queue_t));
  queue->head = NULL;
  queue->tail = NULL;
  queue->size = 0;
  queue->maxSize = maxSize;
  pthread_mutex_init(&queue->m, NULL);
  pthread_cond_init(&queue->cv, NULL);
}

/**
 *  Destroys the queue, freeing any remaining nodes in it.
 */
void queue_destroy(queue_t *queue) { 
  /* Your code here */
  if(queue->head == NULL){
    return;
  }
  queue_node_t* curr = queue->head;
  while(curr != NULL){
    queue_node_t* bye_node = curr;
    bye_node->next = NULL;
    curr = curr->next;
    free(bye_node);
  }
  free(queue);
}
