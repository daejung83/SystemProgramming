//Derek D Kim
//Jan. 21, 2016
//CS 241
//HW0

#include <stdio.h>
#include <limits.h>

int main(){
	//Chapter 2
		//how many bits are there in a byte
		printf("char is a byte and there are %d bits\n", CHAR_BIT);
		printf("char is 1 byte\n");
		printf("Number of bytes for int, double, float, long, long long \n %d, %d, %d, %d, %d \n", sizeof(int), sizeof(double), sizeof(float), sizeof(long), sizeof(long long));
	
		//Follow the int pointer
			//if the address of data is 0x7fbd9d40
			// data+2 would be 0x7fbd9d56

			//also data[3] == 3[data]

		//sizeof character arrays, incrementing pointers
			//char *ptr = "hello";
			//*ptr = 'J';
			//above code will seg fault because *ptr is read only

			printf("sizeof(\"Hello/0World\") will return %d\n", sizeof("Hello\0World"));
			printf("strlen(\"Hello/0World\") will return %d\n", strlen("Hello\0World"));
			printf("example of sizeof(x) = 3 would be sizeof(\"ab\") = %d\n" , sizeof("ab"));
			printf("example of sizeof(y) = 4 or 8 depending on machine would be sizeof(int) = %d\n", sizeof(int));
			//this would depend on if your machine is 32 or 64 bits
			

	return 0;
}
