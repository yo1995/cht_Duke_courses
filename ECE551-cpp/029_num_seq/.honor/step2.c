//This file is for Step 2.
//You should do 
//  Step 2 (A): write seq2
//  Step 2 (B): write main to test seq2
//  Step 2 (C): write sumSeq2
//  Step 2 (D): add test cases to main to test sumSeq2
// 
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int seq2(int x){
	return x*x + 3*x + 1;
}

int sumSeq2(int low, int high){
	int sum = 0;
	int x = low;
	while(x < high){
		sum = sum + seq2(x);
		x++;
	}
	return sum;
}

void main(){
	int x;
	int test_return_value;
	//test 1
	x = -4;
	test_return_value = seq2(x);
	if (test_return_value == 5)
		printf("seq2(%d) = %d\n",x,test_return_value);
	
	x = 0;
	test_return_value = seq2(x);
	if (test_return_value == 1)
		printf("seq2(%d) = %d\n",x,test_return_value);

	x = 14;
	test_return_value = seq2(x);
	if (test_return_value == 239)
		printf("seq2(%d) = %d\n",x,test_return_value);	
		
	//test 2 
	int low = 0;
	int high = 2;
	test_return_value = sumSeq2(low, high);
	printf("sumSeq2(%d, %d) = %d\n",low, high, test_return_value);
	
	low = 3;
	high = 6;
	test_return_value = sumSeq2(low, high);
	printf("sumSeq2(%d, %d) = %d\n",low, high, test_return_value);

	low = 9;
	high = 7;
	test_return_value = sumSeq2(low, high);
	printf("sumSeq2(%d, %d) = %d\n",low, high, test_return_value);

}
