//Derek D Kim
//Jan. 21, 2016
//CS 241
//HW0

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>



	//Chapter 4
		//Memory allocation using malloc, heap and time
			//if you want to use data after the lifetime of the function it was created in
				//then you could use a static data out side of the function or create memory using malloc

				//For every malloc there is a "free"

		//Heap allocation Gotchas
			//malloc would fail if we used up all the memory

			//time would return a time_t and ctime would return a char* where most people can understand

			//free(ptr); free(ptr); is wrong because it is freeing the memory twice

			//free(ptr);
			//printf("%s\n", ptr);
			//above code is incorrect because it is trying to access part of the memory that have been freed

			//above 2 mistakes could be avoided by only releasing the memory once your done with the memory
			//also you could set the ptr to NULL once it has been freed as Dangling Pointer

		//struct, typedefs and a linked list
			struct data{
				char* name;
				int age;
				struct data* friends;
			};

			typedef struct data Person;

			Person* create_person(char*, int);
			void destroy_person(Person*);

			int main(){
				Person* person1 = (Person*) malloc(sizeof(Person));
				Person* person2 = (Person*) malloc(sizeof(Person));

				person1->name = "Agent Smith";
				person2->name = "Sonny Moore";
				person1->age = 128;
				person2->age = 256;
				person1->friends = person2;
				person2->friends = person1;

				printf("%s %s %d %d %s %s \n", person1->name, person2->name, person1->age, person2->age, person1->friends->name, person2->friends->name);

				free(person1);
				free(person2);

			

		//Duplicating strings, memory allocation and deallocation of structures

				Person* person3 = create_person("Agent Smith", 128);
				Person* person4 = create_person("Sonny Moore", 256);

				printf("%s %s %d %d \n", person3->name, person4->name, person3->age, person4->age);

				destroy_person(person3);
				destroy_person(person4);


				return 0;
			}
			Person* create_person(char* p_name, int p_age){
				Person* result = (Person*) malloc(sizeof(Person));
				result->name = strdup(p_name);
				result->age = p_age;
				result->friends = NULL;
				return result;
			}

			void destroy_person(Person* p){
				free(p->name);
				p->name = NULL;
				free(p);
				p = NULL;
			}










