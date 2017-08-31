/**
 * Machine Problem 0
 * CS 241 - Spring 2016
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "part2-functions.h"

/**
 * (Edit this function to print out the "Illinois" lines in
 * part2-functions.c in order.)
 */
int main() {
  // your code here
  //first_step
	first_step(81);
	//second_step
	int a = 132;
	second_step(&a);
	//double_step
	int ** val = (int**)malloc(sizeof(int*));
	val[0] = (int*)malloc(sizeof(int));
	val[0][0] = 8942;
	double_step(val);
	free(val[0]);
	free(val);
	//strange_step
	void* num = malloc(6*sizeof(int));
	void* temp = num + 5;
	*(int*)temp = 15;
	
	strange_step(num);
	free(num);
	//empty_step

	void* empty_string = malloc(20);
	((char*)empty_string)[3] = 0;
	empty_step((void*)empty_string);
	free(empty_string);

	//two_step
	char* two_string = "helu";
	void* two_void = (void*)two_string;
	two_step(two_void, two_string);

	//three_step
	char* three_first = "hello world!";
	char* three_second = three_first + 2;
	char* three_third = three_second + 2;
	three_step(three_first, three_second, three_third);

	//step_step_step
	char* step_first = "Hello World From 17 wow wow";
	char step_second[20];
	char step_third[20];
	strcpy(step_second, "blah blah blah");
	strcpy(step_third, "blah blah blah blah");
	
	step_second[2] = (step_first[1] + 8);
	step_third[3] = (step_second[2] + 8);

	step_step_step(step_first, step_second, step_third);

	//it_may_be_odd
	int odd_num = 5;
	void* odd = &odd_num;

	it_may_be_odd((char*)odd, odd_num);

	//tok_step
	char tok[20] = "Hello,CS241";
	// printf("before tok: %s \n", tok);
	tok_step(tok);

	//the_end
	// char test[2] = "12";
	// test[0] = 'B' - 'A';
	// void* end = (void*)&test;
	// void* end2 = end;
	// int test_int = (*(int*)test)%3;

	// printf("%c, %d\n", test[0], test_int);
	// the_end(end2, end);
	// int* end_int = malloc(sizeof(int));

	//the_end2

	// int end_int = 1013;
	// char* end_char = (char*) &end_int;

	// end_char[0] = 'B' - 'A';

	// void * blue = (void*) end_char;
	// void * orange = (void*) &end_int;
	// printf("orange : blue: %d, %d \n", ((char*)blue)[0] == 1, *((int*)orange)%3);
	// the_end(orange, blue);

	//the_end3
	void* end_ptr = malloc(5*sizeof(int));
	char* end_char = (char*)end_ptr;
	end_char[0] = 'B' - 'A';
	end_char[1] = 'A' - 'A';
	end_char[2] = 'B' - 'A';
	end_char[3] = 'B' - 'A';

	void * blue = (void*) end_char;
	void * orange = (void*) end_ptr;
	// printf("orange : blue: %d, %d \n", ((char*)blue)[0] == 1, *((int*)orange)%3);

	the_end(orange, blue);

	free(end_ptr);
  return 0;
}
