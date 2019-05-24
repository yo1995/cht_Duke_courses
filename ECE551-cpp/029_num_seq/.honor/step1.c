//This file is for Step 1.
//You should do 
//  Step 1 (A): write seq1
//  Step 1 (B): write main to test seq1
//  Step 1 (C): write printSeq1Range
//  Step 1 (D): add test cases to main to test printSeq1Range
// 
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int seq1(int x){
	return 4 * x - 3;
}

void printSeq1Range(int low, int high){
	
//	if (low > high){
//		exit(EXIT_FAILURE); //invalid problem
//	} // already clarified by the instruction.
	int x = low;
	while(x < high){
		if (x == low){
			printf("%d",seq1(x));	//the fisrt to be printed
		}
		else{
			printf(",%d",seq1(x));
		}
		x++;
	}
	printf("\n");
}

void main(){
	int test_return_value;
	int x;
	//test case 1
	x = 0;
	test_return_value = seq1(x);
	if (test_return_value == -3)
		printf("seq1(%d) = %d\n",x,test_return_value);
		
	//test case 2
	x = INT_MIN;//what about underflow?
	test_return_value = seq1(x);
	if (test_return_value == 4*x-3)
		printf("seq1(%d) = %d\n",x,test_return_value);
	
	
	//test print multiple
	
	printf("printSeq1Range(%d,%d)\n",0,0);
	printSeq1Range(0,0);
	
	printf("printSeq1Range(%d,%d)\n",-2,6);
	printSeq1Range(-2,6);
	
	printf("printSeq1Range(%d,%d)\n",7,3);
	printSeq1Range(7,3);
	
	printf("printSeq1Range(%d,%d)\n",0,4);
	printSeq1Range(0,4);
	
//	printf("printSeq1Range(%d,%d)\n",INT_MIN,INT_MAX);
//	printSeq1Range(INT_MIN,INT_MAX);
}
