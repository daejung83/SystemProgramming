///**
// * Machine Problem 1
// * CS 241 - Spring 2016
// */
//
//#include "vector.h"
//#include <stdlib.h>
//#include <stdio.h>
//
//// Test your vector here
//int main() { 
//
//	//test Vector *Vector_create()
//	Vector* n_vec = Vector_create();
//
//	printf("-----testing Vector_create----\n");
//	//printf("vec info: size: %zu, capacity: %zu\n", (*n_vec).size, (*n_vec).capacity);
//  
//  
//  //void Vector_insert(Vector *vector, size_t index, const char *str) {
//  //for(size_t i = 0; i < 10; i++){ 
//  //  Vector_insert(n_vec, i, "asdek");
//  //  printf("for loop insert i: %zu\n", i);
//  //}
//  //Vector_insert(n_vec, 5, "new_ele");
//  //Vector_delete(n_vec, 5);
//  //Vector_delete(n_vec, 9);
//  //Vector_insert(n_vec, 9, "Hello");
//  //Vector_insert(n_vec, 0, "First");
//  //Vector_delete(n_vec, 0);
//  //Vector_insert(n_vec, 0, "Second");
//  //Vector_delete(n_vec, 0);
//  //Vector_append(n_vec,"Append1");
//  //Vector_append(n_vec,"Append2");
//  //Vector_append(n_vec,"Append3");
//  //Vector_append(n_vec,"Append4");
//  //Vector_append(n_vec,"Append5");
//  //Vector_append(n_vec,"Append6");
//  //Vector_append(n_vec,"Append7");
//  //Vector_append(n_vec,"Append8");
//  //Vector_append(n_vec,"Append9");
//  //Vector_append(n_vec,"Append10");
//  //Vector_append(n_vec,"Append11");
//  //Vector_append(n_vec,"Append12");
//  //Vector_append(n_vec,"Append13");
//  //Vector_delete(n_vec, 9);
//  //Vector_insert(n_vec, 17, "lets break my code");
//  //Vector_append(n_vec,"Append14");
//  //Vector_append(n_vec,"Append15");
//  //Vector_delete(n_vec, 8);
//  //Vector_append(n_vec,"Append16");
//  for(int i = 0; i < 1000000; i++){
//    Vector_append(n_vec, "Append oh God!");
//  }
//
//  for(size_t i = 0; i < (*n_vec).size; i++){
//    printf("printf array %zu, %s\n",i,(*n_vec).array[i]);
//  }
//
//  //test void Vector_destroy(Vector *vector)
//  //do this later figure something out you dumbass
//  //Vector_destroy(n_vec);
//  //printf("vector.array[0]: %s\n", (*n_vec).array[0]);
//  //if(n_vec == NULL){
//  //  printf("vector has been destory\n");
//  //}else{
//  //  printf("failed to destory vector\n");
//  //}
//  
//  //test size_t Vector_size(Vector *vector)
//  if((*n_vec).size == Vector_size(n_vec)){
//    printf("Vector_size is correct\n");
//  }else{
//    printf("Vector_size is incorrect\n");
//  }
//  
//  //test void Vector_resize((Vector *vector, size_t new_size)
//  
//
//
//
//  //test void Vector_set(Vector *vecotr, size_t index, const char *str)
//  
//  
//  //test const char *Vector_get(Vector *vector, size_t index, const char *str)
//
//  Vector_destroy(n_vec);
//	return 0; 
//}

#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
void print_vector(Vector * vector) {
  size_t size = Vector_size(vector);
  size_t capacity = vector->capacity;
  printf("vector size: %zu, vector capacity: %zu\n", size, capacity);
  printf("|");
  for (size_t i = 0; i < size; i++) {
    printf("%s|", Vector_get(vector, i));
  }
  for (size_t i = size; i < capacity; i++) {
    printf("__|");
  }
  printf("\n-----------------------------------------\n\n\n");
}

// Create and test your vector here
int main() {
  printf("Creating a new vector\n");
  Vector *vector = Vector_create();
  print_vector(vector);
  printf("Adding 10 elements\n");
  for (size_t i = 0; i < 10; i++) {
    // this gets copied in add, so its fine to declare in loop
    char str[2];
    str[0] = 'A' + i;
    str[1] = '\0';
    printf("\tAdding '%c' at index %zu\n", str[0], i);
    Vector_append(vector,str);
    if(i == 0 || i == 9){
      print_vector(vector);
    }
  }
  printf("Now I am adding an 11th element .. watch the vector capacity increase\n");
  char *c  = "K";
  printf("\tAdding 'K' at index 10\n");
  Vector_append(vector, c);
  print_vector(vector);
  printf("Now I am inserting NULL into index 5\n");
  Vector_insert(vector, 5, NULL);
  print_vector(vector);
  char *something = "something";
  printf("Now I am inserting another thing into index 5\n\t Notice how NULL shifted down instead of overwritten\n");
  Vector_insert(vector, 5, something);
  print_vector(vector);
  printf("Now I am inserting way past the end of the array (at index 42)\n\tNotice how the vector resizes to accommodate the last element\n");
  char *way_past = "way past the end";
  Vector_insert(vector, 42, way_past);
  print_vector(vector);
  printf("Now I will demonstrate the 3 resize scenarios:\n\n\n");
  printf("Resizing to less than size and capacity (15 in this example)\n\tNotice that things got deallocated\n");
  Vector_resize(vector, 15);
  print_vector(vector);
  printf("Resizing to greater than size but less than capacity (21 in this example)\n\tNotice that indices 15 to 21 but padded with NULLS (they are valid elements\n");
  Vector_resize(vector, 21);
  print_vector(vector);
  printf("Resizing to greater than size AND capacity (64 in this example)\n\tNotice that the vector's capacity had to double to accomodate the valid NULLs\n");
  Vector_resize(vector, 64);
  print_vector(vector);
  Vector_destroy(vector);
  return 0;
}
