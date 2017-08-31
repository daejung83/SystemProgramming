/**
 * Machine Problem 4
 * CS 241 - Spring 2016
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


typedef struct memory{
  size_t size;
  struct memory* pre;
  struct memory* next;
  struct memory* f_pre;
  struct memory* f_next;
}memory;

//head nodei
static memory* head = NULL;
//tail node
static memory* tail = NULL;
static memory* f_head = NULL;
static memory* f_tail = NULL;
void* cap;

int node_is_free(memory* node){
  if(node == NULL){
    return 0;
  }
  if(f_head == node || f_tail == node || (node->f_next != NULL && node->f_pre != NULL)){
    return 1;
  }else{
    return 0;
  }
}

/**
 * Allocate space for array in memory
 *
 * Allocates a block of memory for an array of num elements, each of them size
 * bytes long, and initializes all its bits to zero. The effective result is
 * the allocation of an zero-initialized memory block of (num * size) bytes.
 *
 * @param num
 *    Number of elements to be allocated.
 * @param size
 *    Size of elements.
 *
 * @return
 *    A pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory, a
 *    NULL pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
 */
void *calloc(size_t num, size_t size) {
  void* result = malloc(num*size);
  memset(result, 0, num*size);
  return result;
}

/**
 *
 * Allocate memory block
 *
 * Allocates a block of size bytes of memory, returning a pointer to the
 * beginning of the block.  The content of the newly allocated block of
 * memory is not initialized, remaining with indeterminate values.
 *
 * @param size
 *    Size of the memory block, in bytes.
 *
 * @return
 *    On success, a pointer to the memory block allocated by the function.
 *
 *    The type of this pointer is always void*, which can be cast to the
 *    desired type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a null pointer is returned.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
 */

void *malloc(size_t size) {
  size = ((size + 1)>>1)<<1;
  if(head == NULL){
    memory* n_node = NULL;
    if(size > 10){
      n_node = sbrk(size<<8);
    }else{
      n_node = sbrk(size + sizeof(memory));
    }
    void* result = (void*)n_node;
    cap = result + size + sizeof(memory);
    n_node->size = size;
    n_node->pre = NULL;
    n_node->next = NULL;
    n_node->f_next = NULL;
    n_node->f_pre = NULL;
    head = n_node;
    tail = n_node;
    return result + sizeof(memory);
  }
  //search while loop
  memory* curr_node = f_head;
  
  while(curr_node != NULL){
    if(curr_node->size >= size){
      if(curr_node == f_head && curr_node == f_tail){
        f_head = NULL;
        f_tail = NULL;
        return ((void*)curr_node) + sizeof(memory);
      }
      if(curr_node->f_pre != NULL){
        curr_node->f_pre->f_next = curr_node->f_next;
      }
      if(curr_node->f_next != NULL){
        curr_node->f_next->f_pre = curr_node->f_pre;
      }
      if(curr_node == f_head){
        f_head = curr_node->f_next;
      }
      if(curr_node == f_tail){
        f_tail = curr_node->f_pre;
      }
      curr_node->f_next = NULL;
      curr_node->f_pre = NULL;
      if(curr_node == tail){
        curr_node->size = size;
        //brk(((void*)curr_node) + size + sizeof(memory));
        cap = ((void*)curr_node) + size + sizeof(memory);
      }
      return ((void*)curr_node) + sizeof(memory);
    }
    curr_node = curr_node->f_next;
  }
  //end search while loop
  memory* n_node = NULL;
  //printf("sbrk(0) - cap: %ld\n", sbrk(0) - cap);
  if(sbrk(0) - cap > (int)(size+sizeof(memory))){
    n_node = cap; 
    cap = cap + size + sizeof(memory);
  }else if(size < 10){
    //brk(cap);
    n_node = sbrk(size<<8);
    cap = ((void*)n_node) + size + sizeof(memory);
  }else{
    //brk(cap);
    n_node = cap;
    sbrk(size+sizeof(memory));
    cap = ((void*)n_node) + size + sizeof(memory);
  }

  n_node->size = size;
  n_node->pre = tail;
  n_node->next = NULL;
  //if(tail != NULL){
  tail->next = n_node;
  //}
  n_node->f_pre = NULL;
  n_node->f_next = NULL;
  tail = n_node;
  void* result = (void*)n_node;
  return result + sizeof(memory);
}

/**
 * Deallocate space in memory
 *
 * A block of memory previously allocated using a call to malloc(),
 * calloc() or realoc() is deallocated, making it available again for
 * further allocations.
 *
 * Notice that this function leaves the value of ptr unchanged, hence
 * it still points to the same (now invalid) location, and not to the
 * null pointer.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(),
 *    calloc() or realloc() to be deallocated.  If a null pointer is
 *    passed as argument, no action occurs.
 */

void free(void *ptr) {
  if(ptr == NULL){
    return;
  }

  memory* n_node = ptr - sizeof(memory);
  //if(n_node->f_next != NULL && n_node->f_pre != NULL){
  //  printf("double free\n");
  //  return;
  //}
  
  if(f_head == NULL && n_node == tail && n_node->size > 1000){
    cap = (void*)n_node;
    brk((void*)n_node);
    return;
  }
  //printf("hello\n");
  if(n_node == tail && node_is_free(n_node->pre) == 1 && n_node->pre != NULL &&  n_node->size > 10000){
    //printf("are we here\n");
    if(n_node->f_pre != NULL){
      n_node->f_pre->f_next = n_node->f_next;
    }
    if(n_node->f_next != NULL){
      n_node->f_next->f_pre = n_node->f_pre;
    }
    if(n_node == f_tail){
      f_tail = n_node->f_pre;
    }
    if(n_node == f_head){
      f_head = n_node->f_next;
    }
    n_node->f_next = NULL;
    n_node->f_pre = NULL;
    memory* reset_node = n_node->pre;
    if(n_node->pre == head){
      head = NULL;
      tail = NULL;
      n_node->next = NULL;
      n_node->pre = NULL;
    }else{
      tail = n_node->pre->pre;
      n_node->pre->pre = NULL;
      if(tail != NULL){
        tail->next = NULL;
      }
    }
    if(reset_node->f_pre != NULL){
      reset_node->f_pre->f_next = reset_node->f_next;
    }
    if(reset_node->f_next != NULL){
      reset_node->f_next->f_pre = reset_node->f_pre;
    }
    if(reset_node == f_tail){
      f_tail = n_node->f_pre;
    }
    if(reset_node == f_head){
      f_head = n_node->f_next;
    }
    
    reset_node->f_pre = NULL;
    reset_node->f_next = NULL;
    
    cap = (void*)reset_node;
    brk(((void*)reset_node));
    return;
  }

  if(n_node->size > 10000 && n_node == tail){
    if(n_node->f_pre != NULL){
      n_node->f_pre->f_next = n_node->f_next;
    }
    if(n_node->f_next != NULL){
      n_node->f_next->f_pre = n_node->f_pre;
    }
    if(n_node == f_tail){
      f_tail = n_node->f_pre;
    }
    if(n_node == f_head){
      f_head = n_node->f_next;
    }
    n_node->f_pre = NULL;
    n_node->f_next = NULL;
    
    tail = n_node->pre;
    if(tail != NULL){
      tail->next = NULL;
      n_node->pre = NULL;
    }else{
      head = NULL;
    }
    cap = ptr-sizeof(memory);
    brk((ptr-sizeof(memory)));
    return;
  }

  if(f_tail == NULL){
    f_head = n_node;
    f_tail = n_node;
    n_node->f_next = NULL;
    n_node->f_pre = NULL;
  }else{
    f_tail->f_next = n_node;
    n_node->f_pre = f_tail;
    f_tail = n_node;
    n_node->f_next = NULL;
  }
  return;
}


/**
 * Reallocate memory block
 *
 * The size of the memory block pointed to by the ptr parameter is changed
 * to the size bytes, expanding or reducing the amount of memory available
 * in the block.
 *
 * The function may move the memory block to a new location, in which case
 * the new location is returned. The content of the memory block is preserved
 * up to the lesser of the new and old sizes, even if the block is moved. If
 * the new size is larger, the value of the newly allocated portion is
 * indeterminate.
 *
 * In case that ptr is NULL, the function behaves exactly as malloc, assigning
 * a new block of size bytes and returning a pointer to the beginning of it.
 *
 * In case that the size is 0, the memory previously allocated in ptr is
 * deallocated as if a call to free was made, and a NULL pointer is returned.
 *
 * @param ptr
 *    Pointer to a memory block previously allocated with malloc(), calloc()
 *    or realloc() to be reallocated.
 *
 *    If this is NULL, a new block is allocated and a pointer to it is
 *    returned by the function.
 *
 * @param size
 *    New size for the memory block, in bytes.
 *
 *    If it is 0 and ptr points to an existing block of memory, the memory
 *    block pointed by ptr is deallocated and a NULL pointer is returned.
 *
 * @return
 *    A pointer to the reallocated memory block, which may be either the
 *    same as the ptr argument or a new location.
 *
 *    The type of this pointer is void*, which can be cast to the desired
 *    type of data pointer in order to be dereferenceable.
 *
 *    If the function failed to allocate the requested block of memory,
 *    a NULL pointer is returned, and the memory block pointed to by
 *    argument ptr is left unchanged.
 *
 * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
 */
void *realloc(void *ptr, size_t size) {
  size = ((size + 1)>>1)<<1;
  if(ptr == NULL && size != 0){
    ptr = malloc(size);
    return ptr;
  }
  if(size == 0){
    free(ptr);
    return NULL;
  }
  memory* node = ptr - sizeof(memory);
  //some reason this take too long
  //if(size < node->size && node == tail){
  //  node->size = size;
  //  cap = ((void*)node) + size + sizeof(memory);
  //  brk(((void*)node) + size + sizeof(memory));
  //  return ptr;
  //}
  if(size <= node->size){
    return ptr;
  }
  void* result = malloc(size);
  if(result == NULL){
    return NULL;
  }
  //size_t set_size;
  //if(node->size > size){
  //  set_size = size;
  //}else{
  //  set_size = node->size;
  //}
  memcpy(result, ptr, node->size);
  free(ptr);
  return result;
}
