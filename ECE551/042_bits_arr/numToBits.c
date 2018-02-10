#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//void singleNum2bin(uint32_t * nums, int * bits){
//	int i = 0;
//	int j = 0;
//	int temp;
//	int bit_temp[32] = {0};
//	uint32_t* nums_temp = nums;
//	printf("nums_temp = %u\n", *nums_temp);
//	while ( *nums_temp ){
//		*(bit_temp+i)= *nums_temp%2;
//		*nums_temp/=2;
//		i++;
//	}
//	
//	for (j=0;j<32;j++){
//		if (j<32-i){
//			* (bits+j) = 0;
//		}
//		else{
//			* (bits+j) = bit_temp[--i];
//		}
//	}
//}

int getNthBit(uint32_t number, int bit) {
  if (bit <0 || bit >= 32) {
    printf("Bit %d is invalid\n", bit);
    exit (EXIT_FAILURE);
  }
  return (number & (1<<bit)) != 0; //what is this? shift bits by 1 bit and return current bit
  //why !=0 rather than directly °´Î»È¡andÂð£¿ 
}

void numToBits(uint32_t * nums, int nNums, int * bits, int nBits) {
	int i = 0;
	int j = 0;
	if ( nNums * 32 > nBits ){
		printf("Invalid call to numToBits! nBits is %d, nNums is %d\n", nBits,nNums);
		return; //return without doing anything else.
	}
	//printf("nums in upper fun= %u\n", *nums);
	// using the modulate method to obtain binary digits.
	else
	{
		for(i = 0; i < nNums; i++)
		{
			for(j = 0; j < 32; j++)
			{
				bits[i * 32 + j] = getNthBit(nums[i], (31 - j));
			}
		}
	}

	
}

void doTest(uint32_t * nums, int n) {
  int bits[n *32];
  //printf("nums in do test= %u\n", *nums);
  numToBits(nums, n, bits, n*32);
  
  int i;
  for (i =0; i < n; i++) {
    printf(" %9d (%8X) => ", nums[i], nums[i]);
    int j;
    for (j = 0; j < 32; j++) {
      printf("%d", bits[i*32 + j]);
    }
    printf("\n");
  }
}

int main(void) {
  uint32_t array1[] = { 1, 2, 3, 4, 5, 15, 109};
  uint32_t array2[] = { 123456789, 987654321 };
  int bits[7*32-1];
  doTest (array1, 7);
  doTest (array2, 2);
  numToBits(array1,7, bits,7*32-1);
  return EXIT_SUCCESS;
}
