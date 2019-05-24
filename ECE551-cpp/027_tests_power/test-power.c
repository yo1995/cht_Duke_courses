#include <stdio.h>
#include <stdlib.h>

unsigned power (unsigned x, unsigned y);
/*
unsigned my_power (unsigned x, unsigned y){
} 
*/
void run_check(unsigned x, unsigned y, unsigned expected_ans){
	unsigned test_answer = power(x, y);
	//unsigned expected_ans = my_power(x, y);
	//printf("x = %u , y = %u\n",x,y);
	//printf("test ans = %u\n",test_answer);
	//return 0;
	if (test_answer == expected_ans){
		printf("x = %u , y = %u\n",x,y);
		printf("test ans = %u, correct!\n",test_answer);
		//exit(EXIT_SUCCESS);
	}
	else {
		printf("x = %u , y = %u\n",x,y);
		printf("test ans = %u, not right!\n",test_answer);
		exit(EXIT_FAILURE);
	}
		
}

int main(){
	run_check(2,10,1024);
	run_check(1,10,1);
	run_check(8,10,1073741824);
	//run_check(-1,10,1);
	run_check(0,0,1);
	//run_check(1,-1,1);
	run_check(0,1,0);
	//only 4 instances were used. comment the others.
	/*
	run_check(1,1,1);
	run_check(10,0,1);
	run_check(1,1,1);
	//run_check(-1,1,-1);
	//run_check(-1,2,1);
	run_check(8,8,16777216);
	run_check(65536,1,65536);
	//run_check(2,32,4294967296);
	//return EXIT_FAILURE;
	*/
	return EXIT_SUCCESS;
}





