//This file is for Step 3.
//You should do 
//  Step 3 (A): write seq3
//  Step 3 (B): write main to test seq3
//  Step 3 (C): write countEvenInSeq3Range
//  Step 3 (D): add test cases to main to test countEvenInSeq3Range
// 
// Be sure to #include any header files you need!

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int seq3(int x, int y){
	return x*y-3*x+2*y;
}



int countEvenInSeq3Range(xLow, xHi, yLow, yHi){
	int x;
	int y;
	int sum = 0;
	x = xLow;
	while(x< xHi){
		y = yLow;
		while(y< yHi){
			if (!(seq3(x,y) & 1)){ //to check if it's even
				sum++;
				//printf("x,y is %d,%d,result is %d\n",x,y,seq3(x,y));
			}
			y++;
			//printf("y = %d",y);
		}
		x++;
		//printf("x = %d",x);
	}
	return sum;
}

void main(){
	int x;
	int y;
	int test_return_value;
	
	//test 1
	x = 5;
	y = -5;
	test_return_value = seq3(x,y);
	if (test_return_value == -50){
		printf("seq3(%d, %d) = %d\n", x, y, test_return_value);
	}
	
	x = 0;
	y = 0;
	test_return_value = seq3(x,y);
	if (test_return_value == 0){
		printf("seq3(%d, %d) = %d\n", x, y, test_return_value);
	}

	x = -4;
	y = -5;
	test_return_value = seq3(x,y);
	if (test_return_value == 22){
		printf("seq3(%d, %d) = %d\n", x, y, test_return_value);
	}

	x = -4;
	y = 5;
	test_return_value = seq3(x,y);
	if (test_return_value == 2){
		printf("seq3(%d, %d) = %d\n", x, y, test_return_value);
	}


	x = 5;
	y = -4;
	test_return_value = seq3(x,y);
	if (test_return_value == -43){
		printf("seq3(%d, %d) = %d\n", x, y, test_return_value);
	}
	
	//test 2
	test_return_value = countEvenInSeq3Range(0,2,0,3);
	//printf("sum is %d",test_return_value);
	if (test_return_value == 4){
		printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",0,2,0,3,test_return_value);
	}

	test_return_value = countEvenInSeq3Range(3,4,5,6);
	if (test_return_value == 1){
		printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",3,4,5,6,test_return_value);
	}
	
	test_return_value = countEvenInSeq3Range(2,1,0,6);
	if (test_return_value == 0){
		printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",2,1,0,6,test_return_value);
	}
	
	test_return_value = countEvenInSeq3Range(3,5,3,5);
	if (test_return_value == 3){
		printf("countEvenInSeq3Range(%d, %d, %d, %d) = %d\n",3,5,3,5,test_return_value);
	}
}


