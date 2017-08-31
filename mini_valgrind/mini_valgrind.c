#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mini_valgrind.h"

#undef malloc
#undef free

/*
 * Replace normal malloc, this malloc will also create meta data info
 * and insert it to the head of the list.
 * You have to malloc enough size to hold both the size of your allocated
 * and the meta_data structure.
 *
 * @param size
 *	Size of the memory block, in bytes.
 * @param file
 *	Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *	Line in the file that causes memory leak
 *
 * @return
 *	On success, return a pointer to the memory block allocated by
 *	the function. Note that this pointer should return the actual
 *	address the pointer starts and not the meta_data.
 *
 *	If the function fails to allocate the requested block of memory,
 *	return a null pointer.
 */
void *mini_malloc(size_t size, const char *file, size_t line) {
  // your code here
  void* result = malloc(size);
  meta_data* a = malloc(sizeof(meta_data));
  a->address = result;
  insert_meta_data(a , size, file, line);
  if(result != NULL);{
    total_usage = total_usage + size;
    return result;
  }

  return NULL;
}

/*
 * Replace normal free, this free will also delete the node in the list.
 *
 * @param ptr
 *	Pointer to a memory block previously allocated. If a null pointer is
 *	passed, no action occurs.
 */
void mini_free(void *ptr) {
  // your code here
  //if(ptr == NULL){
  //  bad_frees = bad_frees + 1;
  //  return;
  //}
  remove_meta_data(ptr);
}

/*
 * Helper function to insert the malloc ptr node to the list.
 * Accumulate total_usage here.
 *
 * @param md
 * 	Pointer to the meta_data
 * @param size
 *	Size of the memory block, in bytes.
 * @param file
 *	Name of the file that uses mini_valgrind to detect memory leak.
 * @param line
 *	Line in the file that causes memory leak
 */
void insert_meta_data(meta_data *md, size_t size, const char *file,
                      size_t line) {
  /* set value for malloc_info*/
  meta_data* curr = head;
  if(head != NULL){  
    while(curr->next != NULL){
      curr = curr->next;
    }
  }
  md->size = size;
  if(strlen(file) > 128){
    strcpy(md->file_name, file);
  }else{
    strncpy(md->file_name, file, 128);
  }
  md->line_num = line;
  md->next = NULL;
  if(head != NULL){
    curr->next = md;
  }else{
    head = md;
  }
  //printf("head %p\n", head);
}

/*
 * Helper function to remove the free ptr node from the list.
 * Add to total_free here.
 *
 * @param ptr
 *	Pointer to a memory block previously allocated.
 */
void remove_meta_data(void *ptr) {
  /* check if ptr is in the list and delete it from list */
  meta_data* curr = head;
  meta_data* pre = NULL;
  if(head == NULL){
    bad_frees++;
    return;
  }
  //if(head->next == NULL){
  //  total_usage = total_usage - curr->size;
  //  free(head);
  //  head = NULL;
  //  return;
  //}
  while(head != NULL && curr != NULL && ptr != curr->address ){
    pre = curr;
    curr = curr->next;
  }
  
  //printf("curr: %p %p\n", curr, ptr);
  if(curr == NULL){
    bad_frees++;
    return;
  }
  
  if(curr->address == ptr){
    //printf("wtf\n");
    if(pre != NULL){
      pre->next = curr->next;
    }
    //curr->next = NULL;
    //checking for bug
    total_free = total_free + curr->size;
    //if(total_usage < 0)
    //  printf("total_usage is under 0\n");
    if(curr == head && curr->next == NULL){
      head = NULL;
    }else{
      head = curr->next;
    }
    free(curr);
    curr = NULL;
  }
}

/*
 * Deletes all nodes from the list. Called when the program exits so make sure
 * to not count these blocks as freed.
 */
void destroy() {
  // your code here
  meta_data* curr = head;
  meta_data* c_next = NULL;
  if(curr != NULL)
    c_next = curr->next;
  while(curr != NULL){
    //printf("wtf\n");
    if(curr->address != NULL){
      free(curr->address);
    }
    free(curr);
    curr = c_next;
    if(curr != NULL)
      c_next = curr->next;
  }
}

/*
 * Print mini_valgrind leak report. DO NOT TOUCH THIS. You risk failing the
 * autograder if you do.
 */
void print_report() {
  meta_data *leak_info;

  FILE *fp_write = fopen("result.txt", "wt");

  size_t total_leak = 0;

  if (fp_write != NULL) {
    fprintf(fp_write, "Heap report:\n\n");
    leak_info = head;
    while (leak_info != NULL) {
      total_leak += leak_info->size;
      fprintf(fp_write, "\tLeak file %s : line %zu\n", leak_info->file_name,
              leak_info->line_num);
      fprintf(fp_write, "\tLeak size : %zu bytes\n", leak_info->size);
      fprintf(fp_write, "\tLeak memory address : %p\n", leak_info->address);
      fputs("\n", fp_write);
      leak_info = leak_info->next;
    }
    fprintf(fp_write, "\nProgram made %zu bad call(s) to free.\n", bad_frees);
    fprintf(fp_write, "\nHeap summary:\n\n");
    fprintf(fp_write,
            "\tTotal memory usage: %zu bytes, Total memory free: %zu bytes\n",
            total_usage, total_free);
    if (total_leak != 0) {
      fprintf(fp_write, "\tTotal leak: %zu bytes\n", total_leak);
    } else {
      fprintf(fp_write, "\tNo leak, all memory are freed, Congratulations!\n");
    }
    fclose(fp_write);
  }
  destroy();
}
