#include <stdio.h>
#include <stdlib.h>

void int2Str(unsigned int x){
	unsigned rem;
	char s;
	int i = 0;
	int j = 0;
	char * array = NULL;
	while(x != 0 ){
		i++;
		rem = x % 10;
		x = x - rem;
		x = x/10;
		s = '0' + rem;
		array = realloc(array, i * sizeof(*array));
		*(array + i - 1) = s;
	}
	for (j = 0; j < i/2; j++){
		//swap((array + j),(array + i - j - 1));
		rem = *(array + j);
		*(array + j) = *(array + i - j - 1);
		*(array + i - j - 1) = rem;
	}
	array = realloc(array, (i+1) * sizeof(*array));
	*(array + i) = '\0';
	printf("%s",array);
}

int main(){
	unsigned x = 155;
	int2Str(x);
}

