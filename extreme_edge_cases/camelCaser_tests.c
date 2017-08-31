#include "camelCaser_tests.h"
#include "camelCaser.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser (char ** (* camelCaser)(const char *))
{
    // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
   	char* blank = "";	
		//test1
		char* input1 = "Hello . . ? World";
    char** output1 = camelCaser(input1);
    char* answer1_1 = "hello";
		char* answer1_2 = "";
		char* answer1_3 = "";

		if(*output1[0] == *answer1_1 && *output1[1] == *answer1_2 && *output1[2] == *answer1_3){
    	printf("test1 was correct \n");
    }else{
    	printf("test1 failed \n");
			for(int i = 0;i < 3; i++){
				if(*output1[i] != *blank)
					free(output1[i]);
			}
			free(output1);
			return 0;
    }
		for(int i = 0;i < 3; i++){
			if(*output1[i] != *blank)
				free(output1[i]);
		}	
		free(output1);
	
		//test2
		char* input2 = "Hello";
		char** output2 = camelCaser(input2);
		
		if(output2 == NULL){
			printf("test2 was correct \n");
		}else{
			printf("test2 failed \n");
			return 0;
		}

		//test3
		char* input3 = ".?.";
		char** output3 = camelCaser(input3);
		char* answer3_1 = "";
		char* answer3_2 = "";
		char* answer3_3 = "";

		if(*output3[0] == *answer3_1 && *output3[1] == *answer3_2 && *output3[2] == *answer3_3){
			printf("test3 was correct \n");
		}else{
			printf("test3 was incorrect \n");
			for(int i = 0;i < 3; i++){
				if(*output3[i] != *blank)
					free(output3[i]);
			}
			free(output3);
			return 0;
		}
		for(int i = 0; i < 3; i++){
			if(*output3[i] != *blank)
				free(output3[i]);
		}
		free(output3);

		//test4
		char* input4 = "hello .?, world,";
		char** output4 = camelCaser(input4);
		char* answer4_1 = "hello";
		char* answer4_2 = "";
		char* answer4_3 = "";
		char* answer4_4 = "world";

		if(*output4[0] == *answer4_1 && *output4[1] == *answer4_2 && *output4[2] == *answer4_3 && *output4[3] == *answer4_4){
			printf("test4 was correct \n");
		}else{
			printf("test4 was incorrect \n");
			for(int i = 0;i < 3; i++){
				if(*output4[i] != *blank)
					free(output4[i]);
			}
			free(output4);
			return 0;
		}

		for(int i = 0;i < 3; i++){
			if(*output4[i] != *blank)
				free(output4[i]);
		}	
		free(output4);
		
		//test5
		char* input5 = "Ths , , hEss,";
		char** output5 = camelCaser(input5);
		char* answer5_1 = "the";
		char* answer5_2 = "";
		char* answer5_3 = "hess";

		
		if(*output5[0] == *answer5_1 && *output5[1] == *answer5_2 && *output5[2] == *answer5_3){
			printf("test5 was correct \n");
		}else{
			printf("test5 was incorrect \n");
			for(int i = 0;i < 3; i++){
				if(*output5[i] != *blank)
					free(output5[i]);
			}
			free(output5);
			return 0;
		}

		for(int i = 0;i < 3; i++){
			if(*output5[i] != *blank)
				free(output5[i]);
		}	
		free(output5);
		//test6
		char* input6 = ", HELLO , HESS,";
		char** output6 = camelCaser(input6);	
		char* answer6_1 = "";
		char* answer6_2 = "hello";
		char* answer6_3 = "hess";

		if(*output6[0] == *answer6_1 && *output6[1] == *answer6_2 && *output6[2] == *answer6_3){
			printf("test6 was correct \n");
		}else{
			printf("test6 was incorrect \n");
			for(int i = 0;i < 3; i++){
				if(*output6[i] != *blank)
					free(output6[i]);
			}
			free(output6);
			return 0;
		}
		for(int i = 0; i < 3; i++){
			if(*output6[i] != *blank)
				free(output6[i]);
		}
		free(output6);
		
		//test7
		char* input7 = "HELLO,";
		char** output7 = camelCaser(input7);
		char* answer7_1 = "hello";

		if(*output7[0] == *answer7_1){
			printf("test7 was correct \n");
		}else{
			printf("test7 was incorrect \n");
			for(int i = 0;i < 1; i++){
				free(output7[i]);
			}
			free(output7);
			return 0;
		}

		for(int i = 0;i < 1; i++){
			if(*output7[i] != *blank)
				free(output7[i]);
		}	
		free(output7);
		

		//test8
		char* input8 = "hOO?bOO,SEE";
		char** output8 = camelCaser(input8);
		char* answer8_1 = "hoo";
		char* answer8_2 = "boo";	

		if(*output8[0] == *answer8_1 && *output8[1] == *answer8_2){
    	printf("test8 was correct \n");
		}else{
			printf("test8 was incorrect \n");
			for(int i = 0;i < 2; i++){
				if(*output8[i] != *blank)
					free(output8[i]);
			}
			free(output8);
			return 0;
		}

		for(int i = 0;i < 2; i++){
			if(*output8[i] != *blank)
				free(output8[i]);
		}	
		free(output8);
    
		//test9
		char* input9 = "5WORLD 4END,";
		char** output9 = camelCaser(input9);
		char* answer9_1 = "5world4end";
    if(*output9[0] == *answer9_1){
      printf("test9 was correct \n");
    }else{
      printf("test9 was incorrect \n");
      for(int i = 0; i < 1; i++){
        if(*output9[i] != *blank)
          free(output9[i]);
      }
      free(output9);
      return 0;
    }
    
    for(int i = 0; i < 1; i++){
      if(*output9[i] != *blank)
        free(output9[i]);
    }
    free(output9);
	  //test10 empty
    char* input_empty =  "";
    char** output_empty = camelCaser(input_empty);
    if( output_empty == NULL){
      printf("test_empty was correct \n");
    }else{
      printf("test_empty was incorrect \n");
      return 0;
    }
    //NULL
    char* input_null = NULL;
    char** output_null = camelCaser(input_null);
	  if(output_null == NULL){
      printf("test_null was correct \n");
    }else{
      printf("test_null was incorrect \n");
      return 0;
    }
    return 1;
}



