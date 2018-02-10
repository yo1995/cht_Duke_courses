#include <stdlib.h> 

int max(int a, int b){
	return a>b?a:b;
}

size_t maxSeq(int * array, size_t n){
	if (n < 1)
		return 0;
	int i = 0;
	size_t currentMaxLength = 0;
	size_t returnMaxLength = 0;
	int * p = array;
	while (i < n-1){
		if (*(p+i) < *(p+i+1)){ //strictly increasing
			currentMaxLength += 1;
			returnMaxLength = max(currentMaxLength,returnMaxLength);
		}
		else {
			currentMaxLength = 0;
		}
		i++;
	}
	return returnMaxLength+1;
}


