#include <stdio.h>
#include <stdlib.h>

//size_t  maxSeq(int * array, size_t n);

int main(){
	//test case definition part
	int t1[] = {1, 2, 1, 3, 5, 7, 2, 4, 6, 9};
	int n1 = sizeof(t1)/sizeof(t1[0]);
	int t2[] = {1,1,1};
	int n2 = sizeof(t2)/sizeof(t2[0]);
	int t3[] = {1,2,1,2,1,2};
	int n3 = sizeof(t3)/sizeof(t3[0]);
	int t4[] = {1,0,-1,-2,-3};
	int n4 = sizeof(t4)/sizeof(t4[0]);
	//drew given
	int t5[] = { 77, 33, 19, 99, 42, 6, 27, 4};
	int n5 = sizeof(t5)/sizeof(t5[0]);
  	int t6[] = { -3, -42, -99, -1000, -999, -88, -77};
  	int n6 = sizeof(t6)/sizeof(t6[0]);
  	int t7[] = { 425, 59, -3, 77, 0, 36};
  	int n7 = sizeof(t7)/sizeof(t7[0]);
  	//my other thoughts
  	int t8[0] = {};
  	int n8 = sizeof(t8)/sizeof(t8[0]);
  	printf("The size of t8 static array is %d", n8);
  	int t9[] = {1,2,3,4,5,0,1,2,3,4,5,6};
  	int n9 = sizeof(t9)/sizeof(t9[0]);
	//printf("The size of t1 static array is %d", n1);
	
	
	//test ongoing part
	
	size_t a;
	a = maxSeq(t1,n1);
	if (a != 4)
		exit(EXIT_FAILURE);

	a = maxSeq(t2,n2);
	if (a != 1)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t3,n3);
	if (a != 2)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t4,n4);
	if (a != 1)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t5,n5);
	if (a != 2)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t6,n6);
	if (a != 4)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t7,n7);
	if (a != 2)
		exit(EXIT_FAILURE);
	
	a = maxSeq(t8,n8);
	if (a != 0)
		exit(EXIT_FAILURE);

	a = maxSeq(t9,n9);
	if (a != 7)
		exit(EXIT_FAILURE);	
	
	return EXIT_SUCCESS;
	//exit(EXIT_FAILURE);
}
