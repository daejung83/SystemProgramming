//Derek D Kim
//Jan. 21, 2016
//CS 241
//HW0

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(){
	//Chapter 1
		//Hello World (System call style)
		printf("-Hello World (System call style)\n");
		printf("**************************\n");
		write(1, "Hi! My name is \n", 16);
		printf("**************************\n");
		printf("\n\n");
		
		//Hello Standard Error Stream
		printf("-Hello standard Error Stream\n");
		printf("**************************\n");
		int count;
		for(count = 0; count < 4; count++){
			write(1, "***", count);
			printf("\n");
		}
		printf("\n");
		printf("**************************\n");
		printf("\n\n");
		
		//Writing to files
		printf("-Writing to files\n");
		printf("**************************\n");		
		printf("\n");
		mode_t mode = S_IRUSR | S_IWUSR;
		int file = open("test1.txt", O_CREAT | O_RDWR | O_TRUNC, mode);
		write(file, "Hi! My name is \n", 16);
		close(file);
		printf("need to check by cat test1.txt\n");
		printf("\n");
		printf("**************************\n");	
		printf("\n\n");

		//Not everything is a system call
		printf("-Not everything is a system call\n");
		printf("**************************\n");
		printf("\n");
		// mode_t mode = S_IRUSR | S_IWUSR;
		printf("need to check by cat test2.txt\n");
		printf("\n");
		printf("**************************\n");	
		printf("\n\n");
		close(1);
		int file1 = open("test2.txt", O_CREAT | O_RDWR | O_TRUNC, mode);
		printf("Hi! My name is \n");
		close(file1);
			//Difference between write() and printf()
			//write could be controlled on how long or where to write
			//but printf only prints to std output

	//Chapter 2
		

	return 0;
}