/**
 * Machine Problem 1
 * CS 241 - Spring 2016
 */

/* An automatically-expanding array of strings. */
#include "vector.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#define INITIAL_CAPACITY 10

Vector *Vector_create() {
  // your code here
  //printf("sizeof(vec): %lu\n", sizeof(Vector));
  Vector* vec = (Vector*)malloc(sizeof(Vector));
	(*vec).array = (char**)malloc(sizeof(char*)*INITIAL_CAPACITY);
  for(size_t i = 0; i < 10; i++){
    (*vec).array[i] = NULL;
  }
	(*vec).size = 0;
	(*vec).capacity = INITIAL_CAPACITY;
  return vec;
}

void Vector_destroy(Vector *vector) {
  assert(vector);
  // your code here
  for(size_t i = 0; i < (*vector).size; i++){
    if((*vector).array[i] != NULL){
      free((*vector).array[i]);
      (*vector).array[i] = NULL;
    }
  }
  //if((*vector).array != NULL)
  free((*vector).array);
  (*vector).array = NULL;
	free(vector);
}

size_t Vector_size(Vector *vector) {
  assert(vector);
  // your code here
  return (*vector).size;
}

void Vector_resize(Vector *vector, size_t new_size) {
  assert(vector);
  // your code here
  if(new_size == (*vector).size){
    printf("new_size is same\n");
    return;
  }

  if(new_size > (*vector).capacity){
    //change capacity
    if(new_size > (*vector).capacity*2)
      (*vector).capacity = new_size;
    else
      (*vector).capacity = (*vector).capacity*2;
    (*vector).array = (char**)realloc((*vector).array, sizeof(char*)*(*vector).capacity);
    //change new parts to NULL
    for(size_t i = (*vector).size; i < (*vector).capacity; i++){
        (*vector).array[i] = NULL;
    }
  }else if(new_size < (*vector).size){
    //delloc parts being del
    for(size_t i = new_size; i < (*vector).capacity; i++){
      if((*vector).array[i] != NULL){
        free((*vector).array[i]);
        (*vector).array[i] = NULL;
      }
    }
  }
  //set size to new_size
  (*vector).size = new_size;
}

void Vector_set(Vector *vector, size_t index, const char *str) {
  assert(vector);
  // your code here
  //size_t can not be neg no need to check
  if(index >= (*vector).capacity){
    printf("incorrect index\n");
    return;
  }else if(index >= (*vector).size){
    Vector_resize(vector, index+1);
  }
  if(str == NULL){
    if((*vector).array[index] != NULL){
      free((*vector).array[index]);
      (*vector).array[index] = NULL;
    }
  }else if((*vector).array[index] == NULL){
    (*vector).array[index] = (char*)malloc(strlen(str)+1);
    strcpy((*vector).array[index], str);
  }else{
    free((*vector).array[index]);
    (*vector).array[index] = NULL;
    (*vector).array[index] = (char*)malloc(strlen(str)+1);
    strcpy((*vector).array[index], str);
  }
}

const char *Vector_get(Vector *vector, size_t index) {
  assert(vector);
  // your code here
  if(index >= (*vector).size){
    printf("incorrect index value\n");
    return NULL;
  }
  return (*vector).array[index];
}

void Vector_insert(Vector *vector, size_t index, const char *str) {
  assert(vector);
  // your code here
  //if(index > (*vector).size){
  //  printf("index is incorrect\n");
  //  return;
  //}
  size_t old_size = (*vector).size;

  if(index >= (*vector).size){
    Vector_resize(vector, index+1);
  }
  if(index < old_size){
    Vector_resize(vector, (*vector).size+1);
    for(size_t i = (*vector).size; i > index; i--){
      (*vector).array[i] = (*vector).array[i-1];
      (*vector).array[i-1] = NULL;
    }
    (*vector).array[index] = NULL;
  }
  //else if(index > old_size){
  //  for(size_t i = old_size; i < (*vector).size; i++){
  //    (*vector).array[index] = NULL;
  //  }
  //}
  //if(str != NULL){
  //  (*vector).array[index] = (char*)malloc(strlen(str)+1);
  //  strcpy((*vector).array[index], str);
  //}
  Vector_set(vector, index, str);
}

void Vector_delete(Vector *vector, size_t index) {
  assert(vector);
  // your code here, what asserts might you want?
  if(index >= (*vector).size){
    printf("incorrect index value\n");
    return;
  }
  if(index < (*vector).size){
    //delete and shift
    if((*vector).array[index] != NULL){
      free((*vector).array[index]);
      (*vector).array[index] = NULL;
    }
    for(size_t i = index; i < (*vector).size; i++){
      (*vector).array[i] = (*vector).array[i+1];
      (*vector).array[i+1] = NULL;
    }
  }
  //delete
  //(*vector).array[(*vector).size] = NULL;
  //(*vector).size--;
  Vector_resize(vector, (*vector).size-1);
}

void Vector_append(Vector *vector, const char *str) {
  assert(vector);
  // your code here
  Vector_insert(vector, (*vector).size, str);
}
