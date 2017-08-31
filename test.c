int main(){

	char* test1 = ' ';

	char** test = &test1;

	printf("test result: %d \n", isspace(*test));

	return 0;
}
